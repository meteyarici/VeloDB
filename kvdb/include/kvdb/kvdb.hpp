#pragma once

#include <chrono>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace kvdb {

struct Config {
    std::size_t shard_count{4};
    std::size_t max_memory_bytes{0}; // 0 = unlimited
};

class Database {
public:
    explicit Database(const Config& config = {});
    ~Database();

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    Database(Database&&) noexcept;
    Database& operator=(Database&&) noexcept;

    bool set(std::string_view key, std::string_view value);
    std::optional<std::string> get(std::string_view key) const;
    bool del(std::string_view key);

    bool expire(std::string_view key, std::chrono::seconds ttl);

    struct Info {
        std::size_t used_memory{0};
        std::size_t max_memory{0};
        std::size_t keys{0};
    };
    Info get_info() const;

private:
    struct Impl;
    Impl* impl_;
};

} // namespace kvdb


