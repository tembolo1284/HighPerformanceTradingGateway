#include "OrderManager.hpp"
#include <stdexcept>

void OrderManager::createOrder(const std::string& orderId, const std::string& orderDetails) {
    std::lock_guard<std::mutex> lock(mtx_);
    orders_[orderId] = orderDetails;
}

void OrderManager::cancelOrder(const std::string& orderId) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = orders_.find(orderId);
    if (it != orders_.end()) {
        orders_.erase(it);
    } else {
        throw std::runtime_error("Order not found");
    }
}

std::string OrderManager::getOrder(const std::string& orderId) const {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = orders_.find(orderId);
    if (it != orders_.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Order not found");
    }
}

