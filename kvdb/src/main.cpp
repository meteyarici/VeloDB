#include "kvdb/kvdb.hpp"
#include "kvdb/net/server.hpp"

#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>
#include <chrono>

std::atomic<bool> g_running{true};

void signal_handler(int) {
    g_running = false;
}

int main(int argc, char* argv[]) {
    std::uint16_t port = 6379;
    
    if (argc > 1) {
        try {
            port = static_cast<std::uint16_t>(std::stoi(argv[1]));
        } catch (...) {
            std::cerr << "Invalid port number, using default: 6379" << std::endl;
        }
    }

    // Setup signal handlers
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    kvdb::Config config;
    config.shard_count = 4;

    kvdb::Database db{config};
    kvdb::net::Server server(db, port);

    server.start();

    std::cout << "KVDB server running on port " << port << std::endl;
    std::cout << "Press Ctrl+C to stop..." << std::endl;

    // Keep main thread alive
    while (g_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    server.stop();
    return 0;
}

