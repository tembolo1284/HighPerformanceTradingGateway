#ifndef FIXMESSAGEHANDLER_HPP
#define FIXMESSAGEHANDLER_HPP

#include <string>
#include <unordered_map>

class FixMessageHandler {
public:
    // Parse a raw FIX message into a key-value map
    std::unordered_map<std::string, std::string> parseMessage(const std::string& rawMessage);

    // Create a raw FIX message from a key-value map
    std::string createMessage(const std::unordered_map<std::string, std::string>& fields);
};

#endif // FIXMESSAGEHANDLER_HPP

