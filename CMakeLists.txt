# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.14)

# Project name and version
project(HighPerformanceTradingGateway VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required Boost components
find_package(Boost REQUIRED COMPONENTS 
    system 
    thread 
    date_time 
    chrono
)

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

# Define directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/test)
set(EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/examples)

# Create list of source files
set(GATEWAY_SOURCES
    ${SRC_DIR}/FixMessageHandler.cpp
    ${SRC_DIR}/Logger.cpp
    ${SRC_DIR}/MarketDataProcessor.cpp
    ${SRC_DIR}/OrderManager.cpp
    ${SRC_DIR}/ThreadPool.cpp
    ${SRC_DIR}/NetworkServer.cpp
    ${SRC_DIR}/NetworkClient.cpp
)

# Create a static library target
add_library(gateway_lib STATIC ${GATEWAY_SOURCES})

# Find Threads (needed for both main executable and tests)
find_package(Threads REQUIRED)

# Set include directories and link dependencies for the library
target_include_directories(gateway_lib
    PUBLIC 
        ${INCLUDE_DIR}
    PRIVATE
        ${Boost_INCLUDE_DIRS}
)

target_link_libraries(gateway_lib
    PUBLIC
        ${Boost_LIBRARIES}
        Threads::Threads
)

# Add compile options for better warnings and optimization
target_compile_options(gateway_lib
    PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -O3>
        $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -O3>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /O2>
)

# Add the main executable
add_executable(HighPerformanceTradingGateway main.cpp)
target_link_libraries(HighPerformanceTradingGateway PRIVATE gateway_lib)

# Add test executable
add_executable(HighPerformanceTradingGatewayTests
    ${TEST_DIR}/FixMessageHandlerTest.cpp
    ${TEST_DIR}/LoggerTest.cpp
    ${TEST_DIR}/MarketDataProcessorTest.cpp
    ${TEST_DIR}/OrderManagerTest.cpp
)

# Link test executable with our library and Google Test
target_link_libraries(HighPerformanceTradingGatewayTests
    PRIVATE
        gateway_lib
        gtest
        gtest_main
)

# Include GoogleTest module and discover tests
include(GoogleTest)
gtest_discover_tests(HighPerformanceTradingGatewayTests)

# Examples section
# Create FIX client executable
add_executable(fix_client ${EXAMPLES_DIR}/fix_client.cpp)
target_link_libraries(fix_client PRIVATE gateway_lib)

# Copy example data files to build directory
file(COPY ${EXAMPLES_DIR}/data DESTINATION ${CMAKE_BINARY_DIR}/examples)

# Add installation rules
install(TARGETS gateway_lib
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
)

install(TARGETS HighPerformanceTradingGateway fix_client
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${INCLUDE_DIR}/
    DESTINATION include/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.hpp"
)

install(DIRECTORY ${EXAMPLES_DIR}/data/
    DESTINATION share/${PROJECT_NAME}/examples
    FILES_MATCHING PATTERN "*.txt"
)

# Print project setup info
message(STATUS "Project Configuration:")
message(STATUS "======================")
message(STATUS "Project Name: ${PROJECT_NAME}")
message(STATUS "Project Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "")
message(STATUS "Directories:")
message(STATUS "------------")
message(STATUS "Source Directory: ${SRC_DIR}")
message(STATUS "Include Directory: ${INCLUDE_DIR}")
message(STATUS "Test Directory: ${TEST_DIR}")
message(STATUS "Examples Directory: ${EXAMPLES_DIR}")
message(STATUS "")
message(STATUS "Dependencies:")
message(STATUS "------------")
message(STATUS "Boost Version: ${Boost_VERSION}")
message(STATUS "Boost Include Dirs: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost Libraries: ${Boost_LIBRARIES}")
message(STATUS "")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Install Prefix: ${CMAKE_INSTALL_PREFIX}")
