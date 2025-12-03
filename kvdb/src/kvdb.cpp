#include "kvdb/kvdb.hpp"

#include "kvdb/model/value.hpp"

#include <unordered_map>
#include <mutex>

namespace kvdb {

struct Database::Impl {
    Config config;
    mutable std::mutex mutex;
    std::unordered_map<std::string, model::Value> store;
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
    model::Value v;
    v.data.assign(value);
    impl_->store[std::string(key)] = std::move(v);
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
    return impl_->store.erase(std::string(key)) > 0;
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

} // namespace kvdb


