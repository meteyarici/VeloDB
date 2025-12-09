#pragma once

#include <string_view>

namespace kvdb::utils {

void log_info(std::string_view msg);
void log_error(std::string_view msg);

} // namespace kvdb::utils


