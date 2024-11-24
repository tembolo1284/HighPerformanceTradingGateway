# HighPerformanceTradingGateway

HighPerformanceTradingGateway is a low-latency, multithreaded C++ application designed for processing and managing fixed-income trading orders and market data using the FIX protocol. The project emphasizes high performance, modularity, and testability, making it ideal for financial trading systems.

## Features

### Core Features
- **Order Management**: Create, modify, and cancel orders with thread-safe operations
- **Market Data Processing**: Efficiently process real-time market data feeds
- **FIX Protocol Handling**: Parse and generate FIX messages with consistent field ordering
- **Thread Safety**: Utilizes `std::mutex` for concurrency
- **Testing**: Comprehensive unit tests using Google Test (GTest)
- **Scalable Architecture**: Designed with extensibility in mind

### Network Features
- **Client-Server Architecture**: Supports multiple concurrent client connections
- **Asynchronous I/O**: Uses boost::asio for efficient network operations
- **Message Queuing**: Thread-safe message queue for order processing
- **Reconnection Handling**: Automatic client reconnection with configurable retry attempts
- **Statistics Monitoring**: Real-time server statistics including message rates and latency

## Project Structure
```
HighPerformanceTradingGateway/

├── include/                     # Header files

│   ├── FixMessageHandler.hpp    # Handles FIX message parsing and creation

│   ├── OrderManager.hpp         # Manages the lifecycle of orders

│   ├── MarketDataProcessor.hpp  # Processes market data feeds

│   ├── Logger.hpp              # Logging system

│   ├── MessageQueue.hpp        # Thread-safe message queue

│   ├── NetworkServer.hpp       # Server implementation

│   ├── NetworkClient.hpp       # Client implementation

│   └── NetworkTypes.hpp        # Network-related types and configs

├── src/                        # Source files

│   ├── Implementation files (.cpp)

├── test/                       # Test folder

│   ├── Unit tests (.cpp)

├── examples/                   # Example applications

│   ├── fix_client.cpp         # FIX client utility

│   ├── data/                  # Sample data files

│   │   ├── sample_orders.txt  # Example FIX orders

│   │   └── README.md         # Data file documentation

│   └── README.md             # Examples documentation

├── main.cpp                    # Entry point

├── CMakeLists.txt             # Build configuration

└── README.md                  # This file
```

## Prerequisites
- CMake (version 3.14 or higher)
- Boost Libraries (system, thread, date_time, chrono)
- A C++ Compiler supporting C++17 (e.g., g++ or clang++)
- Google Test (GTest)

## Building the Project

1. Install dependencies (Ubuntu/Debian):
```bash
# Install build tools
sudo apt update
sudo apt install cmake build-essential

# Install Boost libraries
sudo apt install libboost-all-dev

# Install Google Test
sudo apt install libgtest-dev
```

2. Configure and build the project:
```bash
# Configure with CMake
cmake -B build

# Build the project
cmake --build build
```

3. Run tests:
```bash
ctest --test-dir build --output-on-failure
```

## Running the Application

### Starting the Server
```bash
./build/HighPerformanceTradingGateway
```

The server provides real-time statistics including:
- Active connections
- Messages processed per second
- Error rates
- Average processing latency

### Using the FIX Client

The project includes a command-line FIX client utility with multiple operation modes:

1. Interactive Mode:
```bash
./build/fix_client -i
```

2. Single Order Mode:
```bash
./build/fix_client -o "35=D|49=SENDER|56=TARGET|11=ORDER123|55=AAPL|54=1|44=150.50|38=100|40=2|"
```

3. Batch File Mode:
```bash
./build/fix_client -f examples/data/sample_orders.txt
```

### FIX Message Format
The system supports standard FIX message fields:
- 35=D : New Order Single
- 49 : SenderCompID
- 56 : TargetCompID
- 11 : ClOrdID (unique order ID)
- 55 : Symbol
- 54 : Side (1=Buy, 2=Sell)
- 44 : Price
- 38 : OrderQty
- 40 : OrdType (2=Limit)

## Testing

The project includes comprehensive test suites:
```bash
# Run all tests
ctest --test-dir build --output-on-failure V

# Run specific test suite
./build/HighPerformanceTradingGatewayTests
```

## Examples

The `examples/` directory contains sample applications and data files demonstrating the gateway's functionality. See [examples/README.md](examples/README.md) for detailed information.

## Future Enhancements
- SSL/TLS support for secure connections
- Market data feed integration
- Order book management
- Risk management system
- Performance monitoring and metrics
- WebSocket interface for real-time updates

