#ifndef FIXMESSAGEHANDLER_HPP
#define FIXMESSAGEHANDLER_HPP

#include <string>
#include <unordered_map>

class FixMessageHandler {
public:
    /**
     * Parse a raw FIX message into a key-value map.
     * @param rawMessage The raw FIX message string (e.g., "35=D|49=Sender|56=Target|").
     * @return A map of key-value pairs representing the FIX fields.
     */
    std::unordered_map<std::string, std::string> parseMessage(const std::string& rawMessage);

    /**
     * Create a raw FIX message from a key-value map.
     * The message is ordered according to a predefined order for specific keys
     * and lexicographically for the rest.
     * @param fields A map of key-value pairs to be encoded into a FIX message.
     * @return The generated FIX message string.
     */
    std::string createMessage(const std::unordered_map<std::string, std::string>& fields);
};

#endif // FIXMESSAGEHANDLER_HPP

