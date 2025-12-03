#pragma once

#include "kvdb/kvdb.hpp"

#include <asio.hpp>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace kvdb::net {

class Server {
public:
    Server(Database& db, std::uint16_t port = 6379);
    ~Server();

    void start();
    void stop();

private:
    void accept_connections();
    void handle_client(std::shared_ptr<asio::ip::tcp::socket> socket);

    Database& db_;
    asio::io_context io_context_;
    asio::ip::tcp::acceptor acceptor_;
    std::vector<std::thread> worker_threads_;
    bool running_{false};
    std::uint16_t port_;
};

} // namespace kvdb::net

