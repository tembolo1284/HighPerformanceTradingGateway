# Trading Gateway Examples

This directory contains example applications and data files demonstrating how to use the High Performance Trading Gateway.

## Directory Structure
- `fix_client.cpp`: A command-line client for sending FIX messages to the server
- `data/`: Sample data files for testing and demonstration
  - `sample_orders.txt`: Example FIX orders
  - `market_data_sample.txt`: Example market data messages

## Building the Examples
The examples are built automatically with the main project:
```bash
cmake -B build
cmake --build build
```

## Running the Examples
1. Start the main server:
```bash
./build/HighPerformanceTradingGateway
```

2. In another terminal, use the FIX client:
```bash
# Interactive mode
./build/fix_client -i

# Send a single order
./build/fix_client -o "35=D|49=SENDER|56=TARGET|11=ORDER123|55=AAPL|54=1|44=150.50|38=100|40=2|"

# Send orders from file
./build/fix_client -f examples/data/sample_orders.txt
```
