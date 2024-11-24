// src/OrderManager.cpp
#include "OrderManager.hpp"
#include "FixMessageHandler.hpp"
#include <iostream>

void OrderManager::createOrder(const std::string& orderId, const std::string& orderDetails) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (activeOrders_.find(orderId) != activeOrders_.end()) {
        throw std::runtime_error("Order ID already exists: " + orderId);
    }
    activeOrders_[orderId] = orderDetails;
}

std::optional<std::string> OrderManager::getOrder(const std::string& orderId) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = activeOrders_.find(orderId);
    if (it != activeOrders_.end()) {
        return it->second;
    }
    return std::nullopt;
}

void OrderManager::processOrder(const std::string& fixMessage) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    FixMessageHandler fixHandler;
    auto fields = fixHandler.parseFixMessage(fixMessage);
    
    // Extract order ID (tag 11 in FIX)
    auto orderIdIt = fields.find("11");
    if (orderIdIt != fields.end()) {
        activeOrders_[orderIdIt->second] = fixMessage;
        std::cout << "Processing order: " << orderIdIt->second << std::endl;
    } else {
        throw std::runtime_error("Missing order ID in FIX message");
    }
}

void OrderManager::cancelOrder(const std::string& orderId) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (activeOrders_.erase(orderId) == 0) {
        throw std::runtime_error("Order not found: " + orderId);
    }
}

void OrderManager::modifyOrder(const std::string& orderId, const std::string& newDetails) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = activeOrders_.find(orderId);
    if (it == activeOrders_.end()) {
        throw std::runtime_error("Order not found: " + orderId);
    }
    it->second = newDetails;
}

bool OrderManager::orderExists(const std::string& orderId) const {
    std::lock_guard<std::mutex> lock(mutex_);
    return activeOrders_.find(orderId) != activeOrders_.end();
}
