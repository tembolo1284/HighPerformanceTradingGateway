// test/OrderManagerTest.cpp
#include <gtest/gtest.h>
#include "OrderManager.hpp"

class OrderManagerTest : public ::testing::Test {
protected:
    OrderManager manager;
};

TEST_F(OrderManagerTest, CreateAndRetrieveOrder_Test) {
    std::string orderId = "ORDER123";
    std::string orderDetails = "35=D|49=SENDER|56=TARGET|11=ORDER123|55=AAPL|54=1|44=150.50|38=100|40=2|";
    
    manager.createOrder(orderId, orderDetails);
    auto retrievedOrder = manager.getOrder(orderId);
    
    ASSERT_TRUE(retrievedOrder.has_value());
    EXPECT_EQ(retrievedOrder.value(), orderDetails);
}

TEST_F(OrderManagerTest, CancelOrder_Test) {
    std::string orderId = "ORDER123";
    std::string orderDetails = "35=D|49=SENDER|56=TARGET|11=ORDER123|55=AAPL|54=1|44=150.50|38=100|40=2|";
    
    manager.createOrder(orderId, orderDetails);
    EXPECT_TRUE(manager.orderExists(orderId));
    
    manager.cancelOrder(orderId);
    EXPECT_FALSE(manager.orderExists(orderId));
    
    auto retrievedOrder = manager.getOrder(orderId);
    EXPECT_FALSE(retrievedOrder.has_value());
}

TEST_F(OrderManagerTest, ModifyOrder_Test) {
    std::string orderId = "ORDER123";
    std::string orderDetails = "35=D|49=SENDER|56=TARGET|11=ORDER123|55=AAPL|54=1|44=150.50|38=100|40=2|";
    std::string newDetails = "35=D|49=SENDER|56=TARGET|11=ORDER123|55=AAPL|54=1|44=160.50|38=200|40=2|";
    
    manager.createOrder(orderId, orderDetails);
    manager.modifyOrder(orderId, newDetails);
    
    auto retrievedOrder = manager.getOrder(orderId);
    ASSERT_TRUE(retrievedOrder.has_value());
    EXPECT_EQ(retrievedOrder.value(), newDetails);
}

TEST_F(OrderManagerTest, DuplicateOrder_Test) {
    std::string orderId = "ORDER123";
    std::string orderDetails = "35=D|49=SENDER|56=TARGET|11=ORDER123|55=AAPL|54=1|44=150.50|38=100|40=2|";
    
    manager.createOrder(orderId, orderDetails);
    EXPECT_THROW(manager.createOrder(orderId, orderDetails), std::runtime_error);
}

TEST_F(OrderManagerTest, CancelNonexistentOrder_Test) {
    std::string orderId = "NONEXISTENT";
    EXPECT_THROW(manager.cancelOrder(orderId), std::runtime_error);
}

TEST_F(OrderManagerTest, ModifyNonexistentOrder_Test) {
    std::string orderId = "NONEXISTENT";
    std::string newDetails = "35=D|49=SENDER|56=TARGET|11=ORDER123|55=AAPL|54=1|44=160.50|38=200|40=2|";
    
    EXPECT_THROW(manager.modifyOrder(orderId, newDetails), std::runtime_error);
}

TEST_F(OrderManagerTest, ProcessFixMessage_Test) {
    std::string fixMessage = "35=D|49=SENDER|56=TARGET|11=ORDER123|55=AAPL|54=1|44=150.50|38=100|40=2|";
    
    EXPECT_NO_THROW(manager.processOrder(fixMessage));
    EXPECT_TRUE(manager.orderExists("ORDER123"));
}
