#ifndef MARKET_DATA_PROCESSOR_HPP
#define MARKET_DATA_PROCESSOR_HPP

#include <string>
#include <vector>

class MarketDataProcessor {
public:
    // Method to process raw market data
    std::vector<std::string> process(const std::string& rawMarketData);
};

#endif // MARKET_DATA_PROCESSOR_HPP

