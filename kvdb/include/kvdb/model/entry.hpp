#pragma once

#include <string>

#include "kvdb/model/value.hpp"

namespace kvdb::model {

struct Entry {
    std::string key;
    Value value;
};

} // namespace kvdb::model


