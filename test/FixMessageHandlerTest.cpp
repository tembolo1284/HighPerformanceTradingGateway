#include "FixMessageHandler.hpp"
#include <gtest/gtest.h>

TEST(FixMessageHandlerTest, ParseMessage) {
    FixMessageHandler handler;
    auto fields = handler.parseMessage("35=D|49=Sender|56=Target|");
    EXPECT_EQ(fields["35"], "D");
    EXPECT_EQ(fields["49"], "Sender");
    EXPECT_EQ(fields["56"], "Target");
}

TEST(FixMessageHandlerTest, CreateMessage) {
    FixMessageHandler handler;
    std::unordered_map<std::string, std::string> fields = {{"35", "D"}, {"49", "Sender"}, {"56", "Target"}};
    std::string message = handler.createMessage(fields);
    EXPECT_EQ(message, "35=D|49=Sender|56=Target|");
}

