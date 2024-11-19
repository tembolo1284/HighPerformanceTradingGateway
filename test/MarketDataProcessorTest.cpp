#include "MarketDataProcessor.hpp"
#include <gtest/gtest.h>

TEST(MarketDataProcessorTest, ProcessMarketData) {
    MarketDataProcessor processor;

    // Raw market data to test
    std::string rawMarketData = "AAPL,150.25,GOOG,2800.50,AMZN,3450.10";

    // Call the process method
    auto processedData = processor.process(rawMarketData);

    // Validate the processed data
    ASSERT_EQ(processedData.size(), 6); // Ensure all 6 items are processed
    EXPECT_EQ(processedData[0], "AAPL");
    EXPECT_EQ(processedData[1], "150.25");
    EXPECT_EQ(processedData[2], "GOOG");
    EXPECT_EQ(processedData[3], "2800.50");
    EXPECT_EQ(processedData[4], "AMZN");
    EXPECT_EQ(processedData[5], "3450.10");
}

