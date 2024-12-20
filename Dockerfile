# Build stage
FROM debian:bookworm-slim AS builder

# Install necessary build tools and dependencies
RUN apt-get update && \
    apt-get install -y \
        cmake \
        ninja-build \
        g++ \
        make \
        git \
        libboost-all-dev && \
    rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /usr/src/HighPerformanceTradingGateway

# Copy all source files
COPY . .

# Build the project using CMake and Ninja
RUN cmake -B build -G Ninja && cmake --build build --target HighPerformanceTradingGateway fix_client

# Runtime stage
FROM debian:bookworm-slim

# Install necessary runtime libraries
RUN apt-get update && \
    apt-get install -y \
        libstdc++6 \
        libboost-system1.74.0 \
        libboost-thread1.74.0 \
        libboost-date-time1.74.0 \
        libboost-chrono1.74.0 && \
    rm -rf /var/lib/apt/lists/*

# Copy the built binaries from the build stage
COPY --from=builder /usr/src/HighPerformanceTradingGateway/build/HighPerformanceTradingGateway /usr/local/bin/HighPerformanceTradingGateway
COPY --from=builder /usr/src/HighPerformanceTradingGateway/build/fix_client /usr/local/bin/fix_client

# Expose the server port
EXPOSE 8080

# Set the entrypoint for the server
ENTRYPOINT ["/usr/local/bin/HighPerformanceTradingGateway"]

