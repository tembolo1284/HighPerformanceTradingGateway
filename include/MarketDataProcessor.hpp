#ifndef MARKETDATAPROCESSOR_HPP
#define MARKETDATAPROCESSOR_HPP

#include <string>
#include <vector>
#include <mutex>

class MarketDataProcessor {
public:
    // Process incoming market data
    void processMarketData(const std::string& data);

    // Get processed data
    const std::vector<std::string>& getProcessedData() const;

private:
    std::vector<std::string> processedData_; // List of processed data
    mutable std::mutex mtx_; // Mutex for thread safety
};

#endif // MARKETDATAPROCESSOR_HPP

