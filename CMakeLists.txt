# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.14)

# Project name and version
project(HighPerformanceTradingGateway VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable testing
enable_testing()

# Include FetchContent module for dependency management
include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
# Configure GoogleTest to use shared CRT on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Define source and include directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)

# Add the main executable
add_executable(HighPerformanceTradingGateway main.cpp
    ${SRC_DIR}/FixMessageHandler.cpp
    ${SRC_DIR}/Logger.cpp
    ${SRC_DIR}/MarketDataProcessor.cpp
    ${SRC_DIR}/OrderManager.cpp
    ${SRC_DIR}/ThreadPool.cpp
)

# Include header files for the main executable
target_include_directories(HighPerformanceTradingGateway PRIVATE ${INCLUDE_DIR})

# Add test executable
add_executable(HighPerformanceTradingGatewayTests
    ${TEST_DIR}/FixMessageHandlerTest.cpp
    ${TEST_DIR}/LoggerTest.cpp
    ${TEST_DIR}/MarketDataProcessorTest.cpp
    ${TEST_DIR}/OrderManagerTest.cpp
    ${SRC_DIR}/FixMessageHandler.cpp
    ${SRC_DIR}/Logger.cpp
    ${SRC_DIR}/MarketDataProcessor.cpp
    ${SRC_DIR}/OrderManager.cpp
    ${SRC_DIR}/ThreadPool.cpp
)

# Link GoogleTest libraries and include headers
target_link_libraries(HighPerformanceTradingGatewayTests PRIVATE gtest gtest_main)
target_include_directories(HighPerformanceTradingGatewayTests PRIVATE ${INCLUDE_DIR})

# Include GoogleTest module and discover tests
include(GoogleTest)
gtest_discover_tests(HighPerformanceTradingGatewayTests)

# Print project setup info
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Source Directory: ${SRC_DIR}")
message(STATUS "Include Directory: ${INCLUDE_DIR}")
message(STATUS "Test Directory: ${TEST_DIR}")
