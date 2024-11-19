#include <gtest/gtest.h>
#include "OrderManager.hpp"

TEST(OrderManagerTest, CreateAndRetrieveOrder) {
    OrderManager manager;
    std::string orderId = "123";
    std::string orderDetails = "Buy 100 shares of XYZ";

    manager.createOrder(orderId, orderDetails);
    auto retrievedOrder = manager.getOrder(orderId);

    EXPECT_EQ(retrievedOrder, orderDetails);
}

TEST(OrderManagerTest, CancelOrder) {
    OrderManager manager;
    std::string orderId = "123";
    std::string orderDetails = "Buy 100 shares of XYZ";
    
    manager.createOrder(orderId, orderDetails);
    manager.cancelOrder(orderId);
    
    // Expect an exception when trying to get a cancelled order
    EXPECT_THROW(manager.getOrder(orderId), std::runtime_error);
}
