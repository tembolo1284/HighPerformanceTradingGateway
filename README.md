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

## Docker Support

The project includes Docker support for easy deployment and testing. The Docker setup provides an isolated environment to build and run the server and client applications, ensuring compatibility across different systems.

### Docker Prerequisites

- Docker (version 20.10 or higher)
- Docker Compose (version 1.29 or higher)

### Docker Files and Scripts

- **Dockerfile**: Builds the server and client applications into Docker images.
- **docker-compose.yml**: Manages the server and client services using Docker Compose.
- **docker-run.sh**: A helper script to simplify building, running, and cleaning up Docker containers.

### Using Docker

#### Build the Docker Images
Run the following command to build the Docker images for the project:
```bash
./docker-run.sh build
```

#### Run the Server
To start the server in a Docker container:
```bash
./docker-run.sh server
```

The server will run on port 8080 by default and provide real-time statistics about its activity.

#### Run the Client
To run the client with a default FIX order:

```bash
./docker-run.sh client
```

By default, the client sends the following order:

```
35=D|49=SENDER|56=TARGET|11=ORDER123|55=AAPL|54=1|44=150.50|38=100|40=2|
```

You can also pass custom arguments to the client. For example:

#### Interactive Mode:

```
./docker-run.sh client -i
```

#### Batch File Mode:

```
./docker-run.sh client -f examples/data/sample_orders.txt
```

#### Clean Up Docker Resources:

To stop and remove all containers, networks, and intermediate Docker layers:

```
./docker-run.sh clean
```

### Docker Compose Services

The docker-compose.yml file defines the following services:

- server: Runs the HighPerformanceTradingGateway server.

- client: Runs the fix_client utility, which can operate in various modes (single order, batch file, or interactive).

By default, the server and client communicate over a custom Docker network (cpp-net), allowing seamless interaction between containers.

### Verifying Docker Setup
To verify that the containers are running correctly:

1. Start the server:
```
./docker-run.sh server
```

2. Start the client:
```
./docker-run.sh client
```
The client should connect to the server and process messages without errors.

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

