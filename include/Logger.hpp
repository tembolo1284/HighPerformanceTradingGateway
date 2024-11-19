#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <mutex>

class Logger {
public:
    // Log a message
    static void log(const std::string& message);

private:
    static std::mutex logMutex_; // Mutex for thread-safe logging
};

#endif // LOGGER_HPP

