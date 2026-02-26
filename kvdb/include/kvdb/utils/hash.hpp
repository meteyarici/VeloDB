#pragma once

#include <cstdint>
#include <string_view>

namespace kvdb::utils {

std::uint64_t hash_key(std::string_view key);

} // namespace kvdb::utils


