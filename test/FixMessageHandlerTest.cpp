#include <gtest/gtest.h>
#include "FixMessageHandler.hpp"

TEST(FixMessageHandlerTest, ParseFixMessage) {
    FixMessageHandler handler;

    std::string fixMessage = "35=D|49=Sender|56=Target|";
    auto fields = handler.parseFixMessage(fixMessage);

    ASSERT_EQ(fields.size(), 3);
    EXPECT_EQ(fields["35"], "D");
    EXPECT_EQ(fields["49"], "Sender");
    EXPECT_EQ(fields["56"], "Target");
}

TEST(FixMessageHandlerTest, BuildFixMessage) {
    FixMessageHandler handler;

    std::unordered_map<std::string, std::string> fields = {
        {"35", "D"},
        {"49", "Sender"},
        {"56", "Target"}
    };

    std::string fixMessage = handler.buildFixMessage(fields);

    // Ensure the output matches the expected FIX message
    EXPECT_EQ(fixMessage, "35=D|49=Sender|56=Target|");
}

