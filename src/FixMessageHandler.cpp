#include "FixMessageHandler.hpp"
#include <sstream>
#include <map>
#include <stdexcept>

std::unordered_map<std::string, std::string> FixMessageHandler::parseFixMessage(const std::string& fixMessage) {
    std::unordered_map<std::string, std::string> fields;
    std::istringstream stream(fixMessage);
    std::string field;

    while (std::getline(stream, field, '|')) {
        auto delimiterPos = field.find('=');
        if (delimiterPos == std::string::npos) {
            throw std::invalid_argument("Invalid FIX field: " + field);
        }

        std::string key = field.substr(0, delimiterPos);
        std::string value = field.substr(delimiterPos + 1);
        fields[key] = value;
    }

    return fields;
}

std::string FixMessageHandler::buildFixMessage(const std::unordered_map<std::string, std::string>& fields) {
    // Use a map to enforce consistent ordering of keys
    std::map<std::string, std::string> orderedFields(fields.begin(), fields.end());
    std::ostringstream oss;

    for (const auto& [key, value] : orderedFields) {
        oss << key << "=" << value << "|";
    }

    return oss.str();
}

