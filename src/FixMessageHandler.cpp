#include "FixMessageHandler.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

// Define the FIX field order
const std::vector<std::string> FIX_FIELD_ORDER = {"35", "49", "56"};

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
    // Order fields explicitly
    std::vector<std::string> orderedKeys;

    // Add fields in FIX_FIELD_ORDER
    for (const auto& key : FIX_FIELD_ORDER) {
        if (fields.find(key) != fields.end()) {
            orderedKeys.push_back(key);
        }
    }

    // Add any remaining keys in lexicographical order
    for (const auto& field : fields) {
        if (std::find(orderedKeys.begin(), orderedKeys.end(), field.first) == orderedKeys.end()) {
            orderedKeys.push_back(field.first);
        }
    }

    // Generate the FIX message
    std::string message;
    for (const auto& key : orderedKeys) {
        message += key + "=" + fields.at(key) + "|";
    }

    return message;
}

