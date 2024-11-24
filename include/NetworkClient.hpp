// include/NetworkClient.hpp
#ifndef NETWORK_CLIENT_HPP
#define NETWORK_CLIENT_HPP

#include <boost/asio.hpp>
#include <string>
#include <future>
#include <memory>
#include "NetworkTypes.hpp"
#include "Logger.hpp"

class NetworkClient {
public:
    explicit NetworkClient(const network::ClientConfig& config,
                         std::shared_ptr<Logger> logger);
    ~NetworkClient();

    // Prevent copying and assignment
    NetworkClient(const NetworkClient&) = delete;
    NetworkClient& operator=(const NetworkClient&) = delete;

    // Asynchronously send a message
    std::future<bool> sendAsync(const network::Message& message);
    
    // Synchronously send a message
    bool send(const network::Message& message);
    
    // Send a file containing messages
    bool sendFile(const std::string& filepath, network::Message::Type type);
    
    // Connect to server
    bool connect();
    
    // Disconnect from server
    void disconnect();
    
    // Check connection status
    bool isConnected() const;
    
    // Get last error
    std::string getLastError() const;

private:
    bool sendInternal(const network::Message& message);
    void handleError(const std::string& error_msg);
    bool reconnect();

    boost::asio::io_context io_context_;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket_;
    std::shared_ptr<Logger> logger_;
    network::ClientConfig config_;
    std::string last_error_;
    std::atomic<bool> connected_{false};
    mutable std::mutex error_mutex_;
};

#endif
