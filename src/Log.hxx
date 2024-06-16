#pragma once

#include <source_location>
#include <string_view>

namespace Log {

extern void Error(
    const std::string_view msg,
    const std::source_location loc = std::source_location::current());

extern void Fatal(
    const std::string_view msg,
    const std::source_location loc = std::source_location::current());

} // namespace Log
