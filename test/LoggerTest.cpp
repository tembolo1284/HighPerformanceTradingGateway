#include "Logger.hpp"
#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

TEST(LoggerTest, LogMessage) {
    // Redirect std::cout to test the logger
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    Logger::log("Test message");
    
    std::cout.rdbuf(oldCout); // Restore std::cout
    EXPECT_EQ(buffer.str(), "Test message\n");
}

