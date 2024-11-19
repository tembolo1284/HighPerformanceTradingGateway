#include "MarketDataProcessor.hpp"

void MarketDataProcessor::processMarketData(const std::string& data) {
    std::lock_guard<std::mutex> lock(mtx_);
    processedData_.push_back(data);
}

const std::vector<std::string>& MarketDataProcessor::getProcessedData() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return processedData_;
}

