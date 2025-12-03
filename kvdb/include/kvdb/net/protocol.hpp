#pragma once

#include "kvdb/kvdb.hpp"

#include <asio.hpp>
#include <string>
#include <vector>

namespace kvdb::net {

// Simple text-based protocol similar to Redis
// Commands: SET key value, GET key, DEL key, EXPIRE key seconds

std::vector<std::string> parse_command(const std::string& line);
std::string execute_command(Database& db, const std::vector<std::string>& args);

} // namespace kvdb::net

