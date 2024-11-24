// src/NetworkServer.cpp
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include "NetworkServer.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <boost/bind.hpp>
#include <chrono>
#include <iostream>
#include <sstream>

NetworkServer::NetworkServer(const network::ServerConfig& config,
                           std::shared_ptr<OrderManager> orderManager,
                           std::shared_ptr<Logger> logger)
    : acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), config.port))
    , order_manager_(std::move(orderManager))
    , logger_(std::move(logger))
    , config_(config) {
    
    message_queue_ = std::make_shared<MessageQueue<network::Message>>();
    worker_threads_.reserve(config.thread_pool_size);
}

NetworkServer::~NetworkServer() {
    stop();
}

void NetworkServer::start() {
    if (running_) {
        logger_->log(Logger::Level::WARNING, "Server already running");
        return;
    }

    running_ = true;
    logger_->log(Logger::Level::INFO, "Starting server on port " + std::to_string(config_.port));
    
    // Start worker threads
    for (size_t i = 0; i < config_.thread_pool_size; ++i) {
        worker_threads_.emplace_back([this] { processMessages(); });
    }
    logger_->log(Logger::Level::INFO, "Started " + std::to_string(config_.thread_pool_size) + " worker threads");

    // Start accepting connections
    startAccept();

    try {
        io_context_.run();
    } catch (const std::exception& e) {
        logger_->log(Logger::Level::ERROR, "IO context error: " + std::string(e.what()));
        handleError(e.what());
    }
}

void NetworkServer::stop() {
    if (!running_) return;

    logger_->log(Logger::Level::INFO, "Stopping server...");
    running_ = false;
    message_queue_->stop();
    
    // Stop accepting new connections
    acceptor_.close();
    io_context_.stop();

    // Wait for all worker threads to finish
    for (auto& thread : worker_threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    worker_threads_.clear();

    logger_->log(Logger::Level::INFO, "Server stopped");
}

void NetworkServer::startAccept() {
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);
    
    acceptor_.async_accept(*socket,
        [this, socket](const boost::system::error_code& error) {
            if (!error) {
                if (stats_.active_connections < config_.max_connections) {
                    handleClient(socket);
                    {
                        std::lock_guard<std::mutex> lock(stats_mutex_);
                        ++stats_.active_connections;
                        logger_->log(Logger::Level::DEBUG, 
                            "New client connected. Active connections: " + 
                            std::to_string(stats_.active_connections));
                    }
                } else {
                    logger_->log(Logger::Level::WARNING, 
                        "Max connections reached (" + 
                        std::to_string(config_.max_connections) + 
                        "), rejecting new connection");
                }
            } else {
                handleError("Accept error: " + error.message());
            }
            
            if (running_) {
                startAccept();
            }
        });
}

void NetworkServer::handleClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    auto buffer = std::make_shared<boost::asio::streambuf>();
    
    boost::asio::async_read_until(*socket, *buffer, '\n',
        [this, socket, buffer](const boost::system::error_code& error, std::size_t bytes_transferred) {
            if (!error) {
                std::string data{
                    boost::asio::buffers_begin(buffer->data()),
                    boost::asio::buffers_begin(buffer->data()) + bytes_transferred - 1
                };
                
                logger_->log(Logger::Level::DEBUG, "Received message: " + data);
                
                // Process the message and get the result
                std::string response = processMessageAndGetResponse(data);
                
                // Send acknowledgment back to client
                sendResponse(socket, response);
                
                // Continue reading from this client
                handleClient(socket);
            } else {
                std::lock_guard<std::mutex> lock(stats_mutex_);
                --stats_.active_connections;
                logger_->log(Logger::Level::DEBUG, 
                    "Client disconnected. Active connections: " + 
                    std::to_string(stats_.active_connections));
            }
        });
}

void NetworkServer::sendResponse(std::shared_ptr<boost::asio::ip::tcp::socket> socket, 
                               const std::string& response) {
    auto response_buffer = std::make_shared<std::string>(response + "\n");
    
    boost::asio::async_write(*socket, 
        boost::asio::buffer(*response_buffer),
        [this, socket, response_buffer](const boost::system::error_code& error, std::size_t /*bytes_transferred*/) {
            if (error) {
                logger_->log(Logger::Level::ERROR, "Failed to send response: " + error.message());
            }
        });
}

std::string NetworkServer::processMessageAndGetResponse(const std::string& data) {
    try {
        auto start_time = std::chrono::steady_clock::now();
        
        network::Message msg(network::Message::Type::FIX, data);
        message_queue_->push(msg);

        // Extract orderId from FIX message for the response
        FixMessageHandler fixHandler;
        auto fields = fixHandler.parseFixMessage(data);
        std::string orderId = fields["11"]; // ClOrdID
        std::string symbol = fields["55"];  // Symbol
        std::string side = fields["54"];    // Side
        std::string quantity = fields["38"]; // OrderQty
        std::string price = fields["44"];    // Price

        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now() - start_time);

        std::ostringstream response;
        response << "ACK|"
                << "OrderID=" << orderId << "|"
                << "Symbol=" << symbol << "|"
                << "Side=" << (side == "1" ? "BUY" : "SELL") << "|"
                << "Quantity=" << quantity << "|"
                << "Price=" << price << "|"
                << "Status=ACCEPTED|"
                << "ProcessingTime=" << duration.count() << "us";

        std::lock_guard<std::mutex> lock(stats_mutex_);
        ++stats_.messages_processed;

        return response.str();

    } catch (const std::exception& e) {
        logger_->log(Logger::Level::ERROR, "Message processing error: " + std::string(e.what()));
        return "NAK|Error=" + std::string(e.what());
    }
}

void NetworkServer::processMessages() {
    while (running_) {
        if (auto message = message_queue_->pop()) {
            try {
                switch (message->type) {
                    case network::Message::Type::FIX:
                        logger_->log(Logger::Level::DEBUG, "Processing FIX message");
                        order_manager_->processOrder(message->payload);
                        break;
                    case network::Message::Type::MARKET_DATA:
                        logger_->log(Logger::Level::DEBUG, "Processing market data message");
                        // Handle market data
                        break;
                    case network::Message::Type::CONTROL:
                        logger_->log(Logger::Level::DEBUG, "Processing control message");
                        // Handle control messages
                        break;
                }
                
            } catch (const std::exception& e) {
                handleError("Message processing error: " + std::string(e.what()));
            }
        }
    }
}

void NetworkServer::handleError(const std::string& error_msg) {
    logger_->log(Logger::Level::ERROR, error_msg);
    std::lock_guard<std::mutex> lock(stats_mutex_);
    ++stats_.errors_encountered;
}

NetworkServer::Statistics NetworkServer::getStatistics() const {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    return stats_;
}
