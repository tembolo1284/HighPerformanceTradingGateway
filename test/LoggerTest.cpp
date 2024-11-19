#include <gtest/gtest.h>
#include "Logger.hpp"

TEST(LoggerTest, LogMessage) {
    Logger logger;
    std::string message = "Test log";
    logger.log(message);
    EXPECT_TRUE(true); // Replace with actual validation logic
}

