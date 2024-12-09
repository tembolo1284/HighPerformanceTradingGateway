// main.cpp
#include <iostream>
#include <iomanip>
#include <memory>
#include <thread>
#include <chrono>
#include <csignal>

#include "FixMessageHandler.hpp"
#include "OrderManager.hpp"
#include "Logger.hpp"
#include "NetworkServer.hpp"
#include "NetworkTypes.hpp"

// Global flag for graceful shutdown
volatile std::sig_atomic_t running = true;

// Signal handler
void signalHandler(int signum) {
    running = false;
}

int main() {
    try {
        // Set up signal handling
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        // Initialize components
        auto logger = std::make_shared<Logger>();
        auto orderManager = std::make_shared<OrderManager>();
        
        // Configure server
        network::ServerConfig serverConfig;
        serverConfig.port = 8080;
        serverConfig.thread_pool_size = 4;
        serverConfig.max_connections = 100;
        serverConfig.client_timeout = std::chrono::milliseconds(5000);

        // Initialize the server
        logger->log(Logger::Level::INFO, "Initializing trading gateway server...");
        logger->log(Logger::Level::INFO, "Configuration:");
        logger->log(Logger::Level::INFO, "  - Port: " + std::to_string(serverConfig.port));
        logger->log(Logger::Level::INFO, "  - Thread Pool Size: " + std::to_string(serverConfig.thread_pool_size));
        logger->log(Logger::Level::INFO, "  - Max Connections: " + std::to_string(serverConfig.max_connections));
        
        NetworkServer server(serverConfig, orderManager, logger);
        
        // Start the server in a separate thread
        std::thread serverThread([&server, &logger]() {
            try {
                server.start();
            } catch (const std::exception& e) {
                logger->log(Logger::Level::FATAL, "Server thread error: " + std::string(e.what()));
            }
        });

        // Example of local FIX message processing
        logger->log(Logger::Level::INFO, "Testing local FIX message processing...");
        FixMessageHandler fixHandler;
        
        // Example new order single message
        std::string sampleOrder = "35=D|49=SENDER|56=TARGET|11=ORDER123|55=AAPL|54=1|44=150.50|38=100|40=2|";
        logger->log(Logger::Level::DEBUG, "Processing sample order: " + sampleOrder);
        
        auto fields = fixHandler.parseFixMessage(sampleOrder);
        
        std::cout << "\nParsed FIX Message Fields:" << std::endl;
        std::cout << "------------------------" << std::endl;
        for (const auto& [key, value] : fields) {
            std::cout << "Tag " << key << ": " << value << std::endl;
        }

        // Process the order locally
        try {
            orderManager->processOrder(sampleOrder);
            logger->log(Logger::Level::INFO, "Sample order processed successfully");
        } catch (const std::exception& e) {
            logger->log(Logger::Level::ERROR, "Failed to process sample order: " + std::string(e.what()));
        }

        // Server monitoring loop
        logger->log(Logger::Level::INFO, "Server running on port " + std::to_string(serverConfig.port));
        logger->log(Logger::Level::INFO, "Press Ctrl+C to stop the server");

        // Statistics tracking variables
        size_t last_messages_processed = 0;
        size_t last_errors = 0;
        auto last_stats_time = std::chrono::steady_clock::now();

        while (running) {
            auto stats = server.getStatistics();
            auto current_time = std::chrono::steady_clock::now();
            auto time_diff = std::chrono::duration_cast<std::chrono::seconds>(
                current_time - last_stats_time).count();

        if (time_diff >= 15) {
            // Calculate message rate and error rate
            size_t new_messages = stats.messages_processed - last_messages_processed;
            size_t new_errors = stats.errors_encountered - last_errors;
            double message_rate = time_diff > 0 ? new_messages / static_cast<double>(time_diff) : 0;
            double error_rate = time_diff > 0 ? new_errors / static_cast<double>(time_diff) : 0;

            // Update display
            std::cout << "\rServer Statistics:"
                     << " Connections: " << stats.active_connections 
                     << " | Messages: " << stats.messages_processed 
                     << " | Msg/sec: " << std::fixed << std::setprecision(1) << message_rate
                     << " | Errors: " << stats.errors_encountered 
                     << " | Err/sec: " << std::fixed << std::setprecision(2) << error_rate
                     << " | Avg Processing: " << stats.average_processing_time.count() << "ms"
                     << std::flush;

            // Update tracking variables
            if (time_diff >= 1) {
                last_messages_processed = stats.messages_processed;
                last_errors = stats.errors_encountered;
                last_stats_time = current_time;
            }
        
            // Log significant changes or issues
            if (new_errors > 0) {
                logger->log(Logger::Level::WARNING, 
                    "Detected " + std::to_string(new_errors) + " new errors in the last period");
            }
        }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Graceful shutdown
        logger->log(Logger::Level::INFO, "Initiating graceful shutdown...");
        server.stop();
        
        if (serverThread.joinable()) {
            serverThread.join();
        }

        logger->log(Logger::Level::INFO, "Server shutdown complete");
        return 0;

    } catch (const std::exception& e) {
        auto logger = std::make_shared<Logger>();
        logger->log(Logger::Level::FATAL, "Fatal error in main: " + std::string(e.what()));
        return 1;
    }
}
