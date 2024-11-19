#include "MarketDataProcessor.hpp"
#include <sstream>

std::vector<std::string> MarketDataProcessor::process(const std::string& rawMarketData) {
    std::vector<std::string> processedData;
    std::istringstream stream(rawMarketData);
    std::string dataPoint;

    // Split raw market data by commas and store in the vector
    while (std::getline(stream, dataPoint, ',')) {
        processedData.push_back(dataPoint);
    }

    return processedData;
}

