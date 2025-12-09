#pragma once

#include <chrono>
#include <cstdint>
#include <string>
#include <optional>

namespace kvdb::model {

struct Value {
    std::string data;
    std::optional<std::chrono::steady_clock::time_point> expire_at;
};

} // namespace kvdb::model


