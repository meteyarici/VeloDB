#include "kvdb/kvdb.hpp"

#include "kvdb/model/value.hpp"

#include <unordered_map>
#include <mutex>

namespace kvdb {

struct Database::Impl {
    Config config;
    mutable std::mutex mutex;
    std::unordered_map<std::string, model::Value> store;
    std::atomic<std::size_t> current_memory_usage{0};
};

Database::Database(const Config& config)
    : impl_(new Impl{config}) {}

Database::~Database() {
    delete impl_;
}

Database::Database(Database&& other) noexcept
    : impl_(other.impl_) {
    other.impl_ = nullptr;
}

Database& Database::operator=(Database&& other) noexcept {
    if (this != &other) {
        delete impl_;
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

bool Database::set(std::string_view key, std::string_view value) {
    std::scoped_lock lock(impl_->mutex);
    
    std::string key_str(key);
    std::size_t new_size = key_str.size() + value.size();
    
    // Check if key exists to update memory usage correctly
    auto it = impl_->store.find(key_str);
    if (it != impl_->store.end()) {
        std::size_t old_size = it->first.size() + it->second.data.size();
        impl_->current_memory_usage -= old_size;
    }

    model::Value v;
    v.data.assign(value);
    impl_->store[std::move(key_str)] = std::move(v);
    impl_->current_memory_usage += new_size;
    
    return true;
}

std::optional<std::string> Database::get(std::string_view key) const {
    std::scoped_lock lock(impl_->mutex);
    auto it = impl_->store.find(std::string(key));
    if (it == impl_->store.end()) {
        return std::nullopt;
    }
    return it->second.data;
}

bool Database::del(std::string_view key) {
    std::scoped_lock lock(impl_->mutex);
    auto it = impl_->store.find(std::string(key));
    if (it == impl_->store.end()) {
        return false;
    }
    
    std::size_t size = it->first.size() + it->second.data.size();
    impl_->current_memory_usage -= size;
    impl_->store.erase(it);
    return true;
}

bool Database::expire(std::string_view key, std::chrono::seconds ttl) {
    std::scoped_lock lock(impl_->mutex);
    auto it = impl_->store.find(std::string(key));
    if (it == impl_->store.end()) {
        return false;
    }
    it->second.expire_at = std::chrono::steady_clock::now() + ttl;
    return true;
}

Database::Info Database::get_info() const {
    std::scoped_lock lock(impl_->mutex);
    return {
        impl_->current_memory_usage,
        impl_->config.max_memory_bytes,
        impl_->store.size()
    };
}

} // namespace kvdb
