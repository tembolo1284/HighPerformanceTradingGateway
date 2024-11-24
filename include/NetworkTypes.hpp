// include/NetworkTypes.hpp
#ifndef NETWORK_TYPES_HPP
#define NETWORK_TYPES_HPP

#include <string>
#include <chrono>

namespace network {
    struct Message {
        enum class Type {
            FIX,
            MARKET_DATA,
            CONTROL
        };

        Type type;
        std::string payload;
        std::chrono::system_clock::time_point timestamp;
        
        Message(Type t, std::string p)
            : type(t)
            , payload(std::move(p))
            , timestamp(std::chrono::system_clock::now())
        {}
    };

    struct ClientConfig {
        std::string host;
        uint16_t port;
        std::chrono::milliseconds timeout{1000};
        size_t retry_attempts{3};
    };

    struct ServerConfig {
        uint16_t port;
        size_t max_connections{1000};
        size_t thread_pool_size{4};
        std::chrono::milliseconds client_timeout{5000};
    };
}

#endif
