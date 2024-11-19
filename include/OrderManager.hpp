#ifndef ORDERMANAGER_HPP
#define ORDERMANAGER_HPP

#include <string>
#include <unordered_map>
#include <mutex>

class OrderManager {
public:
    // Add a new order
    void createOrder(const std::string& orderId, const std::string& orderDetails);

    // Cancel an existing order
    void cancelOrder(const std::string& orderId);

    // Retrieve an order's details
    std::string getOrder(const std::string& orderId) const;

private:
    std::unordered_map<std::string, std::string> orders_; // Map of orders
    mutable std::mutex mtx_; // Mutex for thread safety
};

#endif // ORDERMANAGER_HPP

