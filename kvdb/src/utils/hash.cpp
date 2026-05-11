#include "kvdb/utils/hash.hpp"

#include <xxhash.h>

namespace kvdb::utils {

std::uint64_t hash_key(std::string_view key) {
    return XXH3_64bits(key.data(), key.size());
}

} // namespace kvdb::utils
