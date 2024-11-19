#include "OrderManager.hpp"
#include <gtest/gtest.h>

TEST(OrderManagerTest, CreateAndRetrieveOrder) {
    OrderManager manager;
    manager.createOrder("1", "Buy 100 Bonds at 99.5");
    EXPECT_EQ(manager.getOrder("1"), "Buy 100 Bonds at 99.5");
}

TEST(OrderManagerTest, CancelOrder) {
    OrderManager manager;
    manager.createOrder("1", "Buy 100 Bonds at 99.5");
    manager.cancelOrder("1");
    EXPECT_THROW(manager.getOrder("1"), std::runtime_error);
}
