#include "kvdb/net/protocol.hpp"

#include <sstream>
#include <chrono>
#include <functional>
#include <unordered_map>

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

namespace {

using CommandHandler = std::function<std::string(Database&, const std::vector<std::string>&)>;

const std::unordered_map<std::string, CommandHandler> COMMANDS = {
    {"SET", [](Database& db, const std::vector<std::string>& args) {
        if (args.size() < 3) return std::string("ERROR: SET requires key and value\n");
        bool result = db.set(args[1], args[2]);
        return result ? std::string("OK\n") : std::string("ERROR: Failed to set\n");
    }},
    {"GET", [](Database& db, const std::vector<std::string>& args) {
        if (args.size() < 2) return std::string("ERROR: GET requires key\n");
        auto value = db.get(args[1]);
        return value ? *value + "\n" : std::string("(nil)\n");
    }},
    {"DEL", [](Database& db, const std::vector<std::string>& args) {
        if (args.size() < 2) return std::string("ERROR: DEL requires key\n");
        bool result = db.del(args[1]);
        return result ? std::string("1\n") : std::string("0\n");
    }},
    {"EXPIRE", [](Database& db, const std::vector<std::string>& args) {
        if (args.size() < 3) return std::string("ERROR: EXPIRE requires key and seconds\n");
        try {
            int seconds = std::stoi(args[2]);
            bool result = db.expire(args[1], std::chrono::seconds(seconds));
            return result ? std::string("1\n") : std::string("0\n");
        } catch (...) {
            return std::string("ERROR: Invalid seconds value\n");
        }
    }},
    {"PING", [](Database&, const std::vector<std::string>&) {
        return std::string("PONG\n");
    }},
    {"INFO", [](Database& db, const std::vector<std::string>&) {
        auto info = db.get_info();
        std::stringstream ss;
        ss << "used_memory:" << info.used_memory << "\n";
        ss << "max_memory:" << info.max_memory << "\n";
        ss << "db_keys:" << info.keys << "\n";
        return ss.str();
    }}
};

} // namespace

std::string execute_command(Database& db, const std::vector<std::string>& args) {
    if (args.empty()) {
        return "ERROR: Empty command\n";
    }

    const std::string& cmd = args[0];
    auto it = COMMANDS.find(cmd);
    
    if (it != COMMANDS.end()) {
        return it->second(db, args);
    }

    return "ERROR: Unknown command or invalid arguments\n";
}

} // namespace kvdb::net
