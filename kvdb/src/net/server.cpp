#include "kvdb/net/server.hpp"
#include "kvdb/net/protocol.hpp"

#include <iostream>
#include <sstream>
#include <array>

namespace kvdb::net {

Server::Server(Database& db, std::uint16_t port)
    : db_(db)
    , acceptor_(io_context_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    , port_(port) {
}

Server::~Server() {
    stop();
}

void Server::start() {
    if (running_) {
        return;
    }

    running_ = true;
    std::cout << "KVDB server starting on port " << port_ << std::endl;

    // Start accepting connections
    accept_connections();

    // Start worker threads
    const std::size_t num_threads = std::thread::hardware_concurrency();
    for (std::size_t i = 0; i < num_threads; ++i) {
        worker_threads_.emplace_back([this]() {
            io_context_.run();
        });
    }

    std::cout << "KVDB server started with " << num_threads << " worker threads" << std::endl;
}

void Server::stop() {
    if (!running_) {
        return;
    }

    running_ = false;
    io_context_.stop();

    for (auto& thread : worker_threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    std::cout << "KVDB server stopped" << std::endl;
}

void Server::accept_connections() {
    auto socket = std::make_shared<asio::ip::tcp::socket>(io_context_);
    
    acceptor_.async_accept(*socket, [this, socket](asio::error_code ec) {
        if (!ec) {
            std::cout << "New client connected" << std::endl;
            handle_client(socket);
        }
        
        if (running_) {
            accept_connections();
        }
    });
}

void Server::handle_client(std::shared_ptr<asio::ip::tcp::socket> socket) {
    auto buffer = std::make_shared<std::array<char, 4096>>();
    
    auto read_handler = [this, socket, buffer](asio::error_code ec, std::size_t length) {
        if (ec) {
            if (ec != asio::error::eof && ec != asio::error::operation_aborted) {
                std::cerr << "Read error: " << ec.message() << std::endl;
            }
            return;
        }

        std::string request(buffer->data(), length);
        // Remove trailing newline/carriage return
        while (!request.empty() && (request.back() == '\n' || request.back() == '\r')) {
            request.pop_back();
        }
        
        std::string response = execute_command(db_, parse_command(request));
        
        asio::async_write(*socket, asio::buffer(response),
            [this, socket, buffer](asio::error_code ec, std::size_t) {
                if (ec) {
                    if (ec != asio::error::operation_aborted) {
                        std::cerr << "Write error: " << ec.message() << std::endl;
                    }
                    return;
                }
                
                // Continue reading from this client
                handle_client(socket);
            });
    };
    
    socket->async_read_some(asio::buffer(*buffer), read_handler);
}

} // namespace kvdb::net

