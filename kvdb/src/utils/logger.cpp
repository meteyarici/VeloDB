#include "kvdb/utils/logger.hpp"

#include <iostream>

namespace kvdb::utils {

void log_info(std::string_view msg) {
    std::clog << "[INFO] " << msg << '\n';
}

void log_error(std::string_view msg) {
    std::clog << "[ERROR] " << msg << '\n';
}

} // namespace kvdb::utils

