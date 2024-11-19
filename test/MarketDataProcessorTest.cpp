#include "MarketDataProcessor.hpp"
#include <gtest/gtest.h>

TEST(MarketDataProcessorTest, ProcessMarketData) {
    MarketDataProcessor processor;
    processor.processMarketData("Bond Price: 99.6");
    auto data = processor.getProcessedData();
    ASSERT_EQ(data.size(), 1);
    EXPECT_EQ(data[0], "Bond Price: 99.6");
}

