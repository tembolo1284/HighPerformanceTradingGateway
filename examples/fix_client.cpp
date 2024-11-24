// examples/fix_client.cpp
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "NetworkClient.hpp"
#include "NetworkTypes.hpp"
#include "Logger.hpp"

void printUsage() {
    std::cout << "Usage:\n"
              << "  1. Single order:    fix_client -o \"35=D|49=SENDER|56=TARGET|11=ORDER123|55=AAPL|54=1|44=150.50|38=100|40=2|\"\n"
              << "  2. File input:      fix_client -f orders.txt\n"
              << "  3. Interactive:     fix_client -i\n"
              << "\nFIX Message Format:\n"
              << "  35=D         : New Order Single\n"
              << "  49=SENDER    : SenderCompID\n"
              << "  56=TARGET    : TargetCompID\n"
              << "  11=ORDER123  : ClOrdID (unique order ID)\n"
              << "  55=AAPL      : Symbol\n"
              << "  54=1/2       : Side (1=Buy, 2=Sell)\n"
              << "  44=150.50    : Price\n"
              << "  38=100       : Quantity\n"
              << "  40=2         : OrderType (2=Limit)\n";
}

void displayResponse(const std::string& response) {
    std::cout << "\nServer Response:" << std::endl;
    std::cout << "---------------" << std::endl;
    
    // Split response by '|' and format each field
    std::istringstream iss(response);
    std::string token;
    
    while (std::getline(iss, token, '|')) {
        if (!token.empty()) {
            // Special formatting for processing time
            if (token.find("ProcessingTime=") != std::string::npos) {
                auto pos = token.find("=");
                std::cout << "Processing Time: " 
                         << std::setw(8) << token.substr(pos + 1)
                         << " microseconds" << std::endl;
            }
            // Special formatting for Status
            else if (token.find("Status=") != std::string::npos) {
                auto pos = token.find("=");
                std::cout << "Status: " 
                         << (token.substr(pos + 1) == "ACCEPTED" ? "\033[32m" : "\033[31m")
                         << token.substr(pos + 1)
                         << "\033[0m" << std::endl;
            }
            // Regular field formatting
            else if (token != "ACK" && token != "NAK") {
                std::cout << token << std::endl;
            }
        }
    }
    std::cout << "---------------" << std::endl;
}

std::string generateOrderId() {
    static int orderNum = 0;
    std::stringstream ss;
    ss << "ORD" << std::setfill('0') << std::setw(6) << ++orderNum;
    return ss.str();
}

std::string buildFixMessage(const std::string& symbol, const std::string& side, 
                          double price, int quantity) {
    std::stringstream ss;
    ss << "35=D|"
       << "49=SENDER|"
       << "56=TARGET|"
       << "11=" << generateOrderId() << "|"
       << "55=" << symbol << "|"
       << "54=" << side << "|"
       << "44=" << std::fixed << std::setprecision(2) << price << "|"
       << "38=" << quantity << "|"
       << "40=2|";
    return ss.str();
}

void interactiveMode(NetworkClient& client) {
    std::cout << "Interactive FIX Message Mode\n"
              << "Enter 'help' for message format, 'quit' to exit\n"
              << "Or use the simplified format: <symbol> <side> <price> <quantity>\n"
              << "Example: AAPL BUY 150.50 100\n\n";

    std::string input;
    while (true) {
        std::cout << "\nEnter order (or command): ";
        std::getline(std::cin, input);
        
        if (input == "quit") {
            break;
        }
        
        if (input == "help") {
            printUsage();
            continue;
        }

        std::string messageToSend;
        if (input.find('|') == std::string::npos) {
            // Parse simplified format
            std::istringstream iss(input);
            std::string symbol, side;
            double price;
            int quantity;

            if (iss >> symbol >> side >> price >> quantity) {
                // Convert side to FIX format (BUY=1, SELL=2)
                std::string fixSide = (side == "BUY" || side == "buy") ? "1" : "2";
                messageToSend = buildFixMessage(symbol, fixSide, price, quantity);
            } else {
                std::cout << "Invalid input format. Use: SYMBOL SIDE PRICE QUANTITY\n";
                continue;
            }
        } else {
            messageToSend = input;
        }

        network::Message msg(network::Message::Type::FIX, messageToSend);
        if (client.send(msg)) {
            std::cout << "Message sent successfully\n";
        } else {
            std::cout << "Failed to send message: " << client.getLastError() << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    try {
        auto logger = std::make_shared<Logger>();
        
        // Configure client
        network::ClientConfig config;
        config.host = "localhost";
        config.port = 8080;
        config.timeout = std::chrono::milliseconds(5000);
        config.retry_attempts = 3;

        NetworkClient client(config, logger);

        if (!client.connect()) {
            std::cerr << "Failed to connect to server\n";
            return 1;
        }

        if (argc < 2) {
            printUsage();
            return 1;
        }

        std::string option = argv[1];
        
        if (option == "-i") {
            interactiveMode(client);
        }
        else if (option == "-f" && argc > 2) {
            if (client.sendFile(argv[2], network::Message::Type::FIX)) {
                std::cout << "File processed successfully\n";
            } else {
                std::cout << "Failed to process file: " << client.getLastError() << "\n";
                return 1;
            }
        }
        else if (option == "-o" && argc > 2) {
            network::Message msg(network::Message::Type::FIX, argv[2]);
            if (client.send(msg)) {
                std::cout << "Order sent successfully\n";
            } else {
                std::cout << "Failed to send order: " << client.getLastError() << "\n";
                return 1;
            }
        }
        else {
            printUsage();
            return 1;
        }

        client.disconnect();
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
