#ifndef FIX_MESSAGE_HANDLER_HPP
#define FIX_MESSAGE_HANDLER_HPP

#include <string>
#include <unordered_map>

class FixMessageHandler {
public:
    std::unordered_map<std::string, std::string> parseFixMessage(const std::string& fixMessage);
    std::string buildFixMessage(const std::unordered_map<std::string, std::string>& fields);
};

#endif // FIX_MESSAGE_HANDLER_HPP

