#include "FixMessageHandler.hpp"
#include "OrderManager.hpp"
#include "MarketDataProcessor.hpp"
#include "Logger.hpp"

int main() {
    Logger::log("Starting HighPerformanceTradingGateway");

    // FIX Message Handler Example
    FixMessageHandler fixHandler;
    auto fields = fixHandler.parseMessage("35=D|49=Sender|56=Target|");
    std::string fixMessage = fixHandler.createMessage(fields);
    Logger::log("Generated FIX Message: " + fixMessage);

    // Order Manager Example
    OrderManager orderManager;
    orderManager.createOrder("1", "Buy 100 Bonds at 99.5");
    Logger::log("Order Created: " + orderManager.getOrder("1"));

    // Market Data Processor Example
    MarketDataProcessor marketDataProcessor;
    marketDataProcessor.processMarketData("Bond Price: 99.6");
    Logger::log("Processed Market Data");

    return 0;
}

