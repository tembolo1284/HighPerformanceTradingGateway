#include "Logger.hpp"
#include <iostream>
#include <mutex>

// Static member initialization
std::mutex Logger::logMutex_;

void Logger::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex_);
    std::cout << message << std::endl;
}

