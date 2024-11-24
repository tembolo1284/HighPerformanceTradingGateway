// include/OrderManager.hpp
#ifndef HIGH_PERFORMANCE_TRADING_GATEWAY_ORDER_MANAGER_HPP
#define HIGH_PERFORMANCE_TRADING_GATEWAY_ORDER_MANAGER_HPP

#include <string>
#include <mutex>
#include <unordered_map>
#include <optional>
#include <stdexcept>

class OrderManager {
public:
    // Create a new order
    void createOrder(const std::string& orderId, const std::string& orderDetails);
    
    // Get an existing order
    std::optional<std::string> getOrder(const std::string& orderId) const;
    
    // Process a FIX message containing order information
    void processOrder(const std::string& fixMessage);
    
    // Cancel an existing order
    void cancelOrder(const std::string& orderId);
    
    // Modify an existing order
    void modifyOrder(const std::string& orderId, const std::string& newDetails);
    
    // Check if an order exists
    bool orderExists(const std::string& orderId) const;

private:
    mutable std::mutex mutex_;
    std::unordered_map<std::string, std::string> activeOrders_;
};

#endif
