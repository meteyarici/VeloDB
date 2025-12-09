#include "kvdb/net/protocol.hpp"

#include <sstream>
#include <chrono>

namespace kvdb::net {

std::vector<std::string> parse_command(const std::string& line) {
    std::vector<std::string> args;
    std::istringstream iss(line);
    std::string arg;
    
    while (iss >> arg) {
        args.push_back(arg);
    }
    
    return args;
}

std::string execute_command(Database& db, const std::vector<std::string>& args) {
    if (args.empty()) {
        return "ERROR: Empty command\n";
    }

    const std::string& cmd = args[0];
    
    if (cmd == "SET" && args.size() >= 3) {
        bool result = db.set(args[1], args[2]);
        return result ? "OK\n" : "ERROR: Failed to set\n";
    }
    else if (cmd == "GET" && args.size() >= 2) {
        auto value = db.get(args[1]);
        if (value) {
            return *value + "\n";
        } else {
            return "(nil)\n";
        }
    }
    else if (cmd == "DEL" && args.size() >= 2) {
        bool result = db.del(args[1]);
        return result ? "1\n" : "0\n";
    }
    else if (cmd == "EXPIRE" && args.size() >= 3) {
        try {
            int seconds = std::stoi(args[2]);
            bool result = db.expire(args[1], std::chrono::seconds(seconds));
            return result ? "1\n" : "0\n";
        } catch (...) {
            return "ERROR: Invalid seconds value\n";
        }
    }
    else if (cmd == "PING") {
        return "PONG\n";
    }
    else if (cmd == "INFO") {
        auto info = db.get_info();
        std::stringstream ss;
        ss << "used_memory:" << info.used_memory << "\n";
        ss << "max_memory:" << info.max_memory << "\n";
        ss << "db_keys:" << info.keys << "\n";
        return ss.str();
    }
    else {
        return "ERROR: Unknown command or invalid arguments\n";
    }
}

} // namespace kvdb::net


