#include <iostream>
#include <unordered_map>
#include "FixMessageHandler.hpp"

int main() {
    FixMessageHandler fixHandler;

    // Parsing a FIX message
    auto fields = fixHandler.parseFixMessage("35=D|49=Sender|56=Target|");

    // Output the parsed fields
    for (const auto& [key, value] : fields) {
        std::cout << key << ": " << value << std::endl;
    }

    // Building a FIX message
    std::string fixMessage = fixHandler.buildFixMessage(fields);

    // Output the FIX message
    std::cout << "FIX Message: " << fixMessage << std::endl;

    return 0;
}

