#!/bin/bash

# Function to stop and remove existing containers
cleanup() {
    echo "Cleaning up existing containers..."
    docker-compose down
    docker network rm cpp-net 2>/dev/null || true
    docker system prune -f
}

# Function to ensure network exists
ensure_network() {
    if ! docker network inspect cpp-net >/dev/null 2>&1; then
        echo "Creating cpp-net network..."
        docker network create cpp-net
    fi
}

# Function to build the Docker image
build_image() {
    echo "Building Docker image..."
    ensure_network
    docker-compose build
}

# Function to run the server
run_server() {
    echo "Starting server..."
    ensure_network
    docker-compose up server
}

# Function to run the client
run_client() {
    echo "Starting client..."
    ensure_network
    docker-compose run --rm client "$@"
}

# Main script
case "$1" in
    "build")
        build_image
        ;;
    "server")
        cleanup
        run_server
        ;;
    "client")
        shift
        run_client "$@"
        ;;
    "clean")
        cleanup
        ;;
    *)
        echo "Usage: $0 {build|server|client|clean} [client args]"
        echo "  build  - Build the Docker image"
        echo "  server - Run the server"
        echo "  client - Run the client with optional arguments"
        echo "  clean  - Clean up containers and networks"
        exit 1
        ;;
esac

