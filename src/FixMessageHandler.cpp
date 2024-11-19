#include "FixMessageHandler.hpp"
#include <sstream>

std::unordered_map<std::string, std::string> FixMessageHandler::parseMessage(const std::string& rawMessage) {
    std::unordered_map<std::string, std::string> fields;
    std::stringstream ss(rawMessage);
    std::string keyValuePair;

    while (std::getline(ss, keyValuePair, '|')) {
        size_t delimiter = keyValuePair.find('=');
        if (delimiter != std::string::npos) {
            std::string key = keyValuePair.substr(0, delimiter);
            std::string value = keyValuePair.substr(delimiter + 1);
            fields[key] = value;
        }
    }

    return fields;
}

std::string FixMessageHandler::createMessage(const std::unordered_map<std::string, std::string>& fields) {
    std::string message;
    for (const auto& field : fields) {
        message += field.first + "=" + field.second + "|";
    }
    return message;
}

