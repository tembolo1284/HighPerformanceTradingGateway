// test/LoggerTest.cpp
#include <gtest/gtest.h>
#include "Logger.hpp"

TEST(LoggerTest, LogMessage_Test) {
    Logger logger;
    std::string message = "Test message";
    logger.log(Logger::Level::INFO, message);
}

TEST(LoggerTest, LogLevels_Test) {
    Logger logger;
    logger.log(Logger::Level::DEBUG, "Debug message");
    logger.log(Logger::Level::INFO, "Info message");
    logger.log(Logger::Level::WARNING, "Warning message");
    logger.log(Logger::Level::ERROR, "Error message");
    logger.log(Logger::Level::FATAL, "Fatal message");
}
