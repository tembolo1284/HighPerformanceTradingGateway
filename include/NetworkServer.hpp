// include/NetworkServer.hpp
#ifndef NETWORK_SERVER_HPP
#define NETWORK_SERVER_HPP

#include <boost/asio.hpp>
#include <memory>
#include <atomic>
#include <vector>
#include <thread>
#include "MessageQueue.hpp"
#include "NetworkTypes.hpp"
#include "OrderManager.hpp"
#include "FixMessageHandler.hpp"
#include "Logger.hpp"

class NetworkServer {
public:
    explicit NetworkServer(const network::ServerConfig& config, 
                         std::shared_ptr<OrderManager> orderManager,
                         std::shared_ptr<Logger> logger);
    ~NetworkServer();

    // Prevent copying and assignment
    NetworkServer(const NetworkServer&) = delete;
    NetworkServer& operator=(const NetworkServer&) = delete;

    // Start the server
    void start();
    
    // Stop the server gracefully
    void stop();
    
    // Get current statistics
    struct Statistics {
        size_t active_connections{0};
        size_t messages_processed{0};
        size_t errors_encountered{0};
        std::chrono::milliseconds average_processing_time{0};
    };
    Statistics getStatistics() const;

private:
    void startAccept();
    void handleClient(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
    void processMessages();
    void handleError(const std::string& error_msg);
    
    // New methods for handling responses
    void sendResponse(std::shared_ptr<boost::asio::ip::tcp::socket> socket, 
                     const std::string& response);
    std::string processMessageAndGetResponse(const std::string& data);

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::shared_ptr<OrderManager> order_manager_;
    std::shared_ptr<Logger> logger_;
    std::shared_ptr<MessageQueue<network::Message>> message_queue_;
    std::vector<std::thread> worker_threads_;
    std::atomic<bool> running_{false};
    mutable std::mutex stats_mutex_;
    Statistics stats_;
    network::ServerConfig config_;
};

#endif
