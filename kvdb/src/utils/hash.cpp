#include "kvdb/utils/hash.hpp"

#include <functional>

namespace kvdb::utils {

std::uint64_t hash_key(std::string_view key) {
    // Placeholder: can be replaced with xxHash later.
    return std::hash<std::string_view>{}(key);
}

} // namespace kvdb::utils


