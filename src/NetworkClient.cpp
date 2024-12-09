// src/NetworkClient.cpp
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include "NetworkClient.hpp"
#include <fstream>
#include <boost/asio/deadline_timer.hpp>
#include <chrono>
#include <thread>

NetworkClient::NetworkClient(const network::ClientConfig& config,
                           std::shared_ptr<Logger> logger)
    : logger_(std::move(logger)), config_(config) {
    // Use SERVER_HOST environment variable if available, otherwise default to localhost
    const char* server_host_env = std::getenv("SERVER_HOST");
    config_.host = server_host_env ? std::string(server_host_env) : "localhost";

    socket_ = std::make_unique<boost::asio::ip::tcp::socket>(io_context_);
    logger_->log(Logger::Level::DEBUG, "NetworkClient initialized with host: " +
                 config_.host + ", port: " + std::to_string(config_.port));
}

NetworkClient::~NetworkClient() {
    disconnect();
}

bool NetworkClient::connect() {
    if (connected_) {
        logger_->log(Logger::Level::DEBUG, "Already connected to server");
        return true;
    }

    try {
        logger_->log(Logger::Level::INFO, "Attempting to connect to " +
                     config_.host + ":" + std::to_string(config_.port));

        boost::asio::ip::tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(config_.host, std::to_string(config_.port));

        boost::system::error_code error;
        boost::asio::connect(*socket_, endpoints, error);

        if (!error) {
            connected_ = true;
            logger_->log(Logger::Level::INFO, "Successfully connected to server");
            return true;
        }

        handleError("Connection error: " + error.message());
        return false;

    } catch (const std::exception& e) {
        handleError("Connection exception: " + std::string(e.what()));
        return false;
    }
}

void NetworkClient::disconnect() {
    if (!connected_) {
        logger_->log(Logger::Level::DEBUG, "Already disconnected");
        return;
    }

    try {
        logger_->log(Logger::Level::INFO, "Disconnecting from server");
        socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket_->close();
        connected_ = false;
        logger_->log(Logger::Level::INFO, "Successfully disconnected from server");
    } catch (const std::exception& e) {
        logger_->log(Logger::Level::ERROR, "Disconnect error: " + std::string(e.what()));
        connected_ = false;
    }
}

bool NetworkClient::send(const network::Message& message) {
    if (!connected_ && !reconnect()) {
        return false;
    }

    try {
        logger_->log(Logger::Level::DEBUG, "Sending message of type: " +
                     std::to_string(static_cast<int>(message.type)));
        return sendInternal(message);
    } catch (const std::exception& e) {
        handleError("Send error: " + std::string(e.what()));
        return false;
    }
}

std::future<bool> NetworkClient::sendAsync(const network::Message& message) {
    return std::async(std::launch::async, [this, message]() {
        logger_->log(Logger::Level::DEBUG, "Starting async message send");
        bool result = send(message);
        logger_->log(Logger::Level::DEBUG, "Async send completed with result: " +
                     std::string(result ? "success" : "failure"));
        return result;
    });
}

bool NetworkClient::sendFile(const std::string& filepath, network::Message::Type type) {
    logger_->log(Logger::Level::INFO, "Attempting to send file: " + filepath);

    std::ifstream file(filepath);
    if (!file) {
        handleError("Failed to open file: " + filepath);
        return false;
    }

    size_t lineCount = 0;
    size_t successCount = 0;
    std::string line;

    while (std::getline(file, line)) {
        ++lineCount;
        if (line.empty()) {
            continue;
        }

        network::Message message(type, line);
        if (send(message)) {
            ++successCount;
        } else {
            logger_->log(Logger::Level::ERROR, "Failed to send line " +
                         std::to_string(lineCount) + " from file");
        }

        // Add a small delay to prevent overwhelming the server
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    logger_->log(Logger::Level::INFO,
        "File sending completed. Successfully sent " + std::to_string(successCount) +
        " of " + std::to_string(lineCount) + " lines");

    return successCount == lineCount;
}

bool NetworkClient::sendInternal(const network::Message& message) {
    try {
        // Prepare the message with a delimiter
        std::string data = message.payload + "\n";

        // Send the message
        boost::system::error_code error;
        boost::asio::write(*socket_, boost::asio::buffer(data), error);

        if (error) {
            handleError("Write error: " + error.message());
            connected_ = false;
            return false;
        }

        // Wait for and read the response
        boost::asio::streambuf response_buffer;
        boost::asio::read_until(*socket_, response_buffer, '\n', error);

        if (error) {
            handleError("Read error: " + error.message());
            return false;
        }

        // Process the response
        std::string response{
            boost::asio::buffers_begin(response_buffer.data()),
            boost::asio::buffers_begin(response_buffer.data()) +
                response_buffer.size() - 1  // -1 to remove newline
        };

        // Log and handle the response
        if (response.substr(0, 3) == "ACK") {
            logger_->log(Logger::Level::INFO, "Server acknowledged message: " + response);
            return true;
        } else {
            logger_->log(Logger::Level::ERROR, "Server rejected message: " + response);
            return false;
        }

    } catch (const std::exception& e) {
        handleError("Send internal error: " + std::string(e.what()));
        connected_ = false;
        return false;
    }
}

bool NetworkClient::reconnect() {
    logger_->log(Logger::Level::INFO, "Attempting to reconnect...");

    for (size_t attempt = 0; attempt < config_.retry_attempts; ++attempt) {
        logger_->log(Logger::Level::DEBUG, "Reconnection attempt " +
                     std::to_string(attempt + 1) + " of " +
                     std::to_string(config_.retry_attempts));

        if (connect()) {
            logger_->log(Logger::Level::INFO, "Reconnection successful");
            return true;
        }

        // Wait before retrying, with exponential backoff
        std::this_thread::sleep_for(std::chrono::milliseconds(100 * (1 << attempt)));
    }

    logger_->log(Logger::Level::ERROR, "Failed to reconnect after " +
                 std::to_string(config_.retry_attempts) + " attempts");
    return false;
}

bool NetworkClient::isConnected() const {
    return connected_;
}

std::string NetworkClient::getLastError() const {
    std::lock_guard<std::mutex> lock(error_mutex_);
    return last_error_;
}

void NetworkClient::handleError(const std::string& error_msg) {
    std::lock_guard<std::mutex> lock(error_mutex_);
    last_error_ = error_msg;
    logger_->log(Logger::Level::ERROR, error_msg);
}

