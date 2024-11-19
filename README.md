# HighPerformanceTradingGateway

HighPerformanceTradingGateway is a low-latency, multithreaded C++ application designed for processing and managing fixed-income trading orders and market data using the FIX protocol. The project emphasizes high performance, modularity, and testability, making it ideal for financial trading systems.

---

## Features

- **Order Management**: Create, modify, and cancel orders with thread-safe operations.
- **Market Data Processing**: Efficiently process real-time market data feeds.
- **FIX Protocol Handling**: Parse and generate FIX messages with consistent field ordering.
- **Thread Safety**: Utilizes `std::mutex` for concurrency.
- **Testing**: Comprehensive unit tests using Google Test (GTest).
- **Scalable Architecture**: Designed with extensibility in mind for additional financial features.

---

## Project Structure

```plaintext
HighPerformanceTradingGateway/

├── include/                     # Header files

│   ├── FixMessageHandler.hpp    # Handles FIX message parsing and creation

│   ├── OrderManager.hpp         # Manages the lifecycle of orders

│   ├── MarketDataProcessor.hpp  # Processes market data feeds

│   ├── Logger.hpp               # Logs application events

│   └── ThreadPool.hpp           # Manages a pool of worker threads (optional)

├── src/                         # Source files

│   ├── FixMessageHandler.cpp

│   ├── OrderManager.cpp

│   ├── MarketDataProcessor.cpp

│   ├── Logger.cpp

│   └── ThreadPool.cpp

├── test/                        # Test folder

│   ├── FixMessageHandlerTest.cpp

│   ├── OrderManagerTest.cpp

│   ├── MarketDataProcessorTest.cpp

│   └── LoggerTest.cpp

├── main.cpp                     # Entry point of the application

├── CMakeLists.txt               # Build configuration file

└── README.md                    # Project documentation
```

## Build and Run

### Prerequisites

 - CMake (version 3.14 or higher)
 - Ninja Build System (optional, but recommended)
 - A C++ Comiler (e.g., g++ or clang++)
 - Google Test (GTest)

### Steps

1. Configure the Build:

```
cmake -B build -G Ninja
```

2. Build the Project:

```
cmake --build build
```

3. Run the executable:

```
./build/HighPeformanceTradingGateway
```

4. Run Tests:

```
ctest --test-dir build --output-on-failure
```

