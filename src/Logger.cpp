// src/Logger.cpp
#include "Logger.hpp"
#include <iostream>
#include <ctime>

std::string Logger::levelToString(Level level) {
    switch (level) {
        case Level::DEBUG:   return "DEBUG";
        case Level::INFO:    return "INFO";
        case Level::WARNING: return "WARNING";
        case Level::ERROR:   return "ERROR";
        case Level::FATAL:   return "FATAL";
        default:            return "UNKNOWN";
    }
}

void Logger::log(Level level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Get current time
    auto now = std::time(nullptr);
    auto* timeinfo = std::localtime(&now);
    char timestamp[80];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    std::cout << "[" << timestamp << "] [" << levelToString(level) << "] " 
              << message << std::endl;
}
