// include/Logger.hpp
#ifndef HIGH_PERFORMANCE_TRADING_GATEWAY_LOGGER_HPP
#define HIGH_PERFORMANCE_TRADING_GATEWAY_LOGGER_HPP

#include <string>
#include <mutex>

class Logger {
public:
    enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    void log(Level level, const std::string& message);

private:
    std::mutex mutex_;
    static std::string levelToString(Level level);
};

#endif // HIGH_PERFORMANCE_TRADING_GATEWAY_LOGGER_HPP

