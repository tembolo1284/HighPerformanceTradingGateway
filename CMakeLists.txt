# Minimum CMake version
cmake_minimum_required(VERSION 3.14)

# Project name and version
project(HighPerformanceTradingGateway VERSION 1.0 LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source files for the main application
set(SOURCES
    src/FixMessageHandler.cpp
    src/OrderManager.cpp
    src/MarketDataProcessor.cpp
    src/Logger.cpp
    src/ThreadPool.cpp
    main.cpp
)

# Main application executable
add_executable(HighPerformanceTradingGateway ${SOURCES})

# Include directories for the main application
target_include_directories(HighPerformanceTradingGateway PRIVATE include)

# Compiler warnings
if (MSVC)
    target_compile_options(HighPerformanceTradingGateway PRIVATE /W4 /WX)
else()
    target_compile_options(HighPerformanceTradingGateway PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# Enable testing
enable_testing()

# Find Google Test
find_package(GTest REQUIRED)

# Test source files
set(TEST_SOURCES
    test/FixMessageHandlerTest.cpp
    test/OrderManagerTest.cpp
    test/MarketDataProcessorTest.cpp
    test/LoggerTest.cpp
)

# Test executable
add_executable(HighPerformanceTradingGatewayTests ${TEST_SOURCES})

# Link libraries for tests
target_link_libraries(HighPerformanceTradingGatewayTests
    PRIVATE
    GTest::GTest
    GTest::Main
    pthread
)

# Include directories for tests
target_include_directories(HighPerformanceTradingGatewayTests PRIVATE include)

# Add tests to the test runner
add_test(NAME FixMessageHandlerTest COMMAND HighPerformanceTradingGatewayTests)
add_test(NAME OrderManagerTest COMMAND HighPerformanceTradingGatewayTests)
add_test(NAME MarketDataProcessorTest COMMAND HighPerformanceTradingGatewayTests)
add_test(NAME LoggerTest COMMAND HighPerformanceTradingGatewayTests)

