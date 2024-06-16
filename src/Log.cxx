#include "Log.hxx"
#include <iostream>

namespace Log {

void Error(const std::string_view msg, const std::source_location loc)
{
    std::cerr << std::format("{}:{}: {}", loc.file_name(), loc.line(), msg);
}

void Fatal(const std::string_view msg, const std::source_location loc)
{
    Error(msg, loc);
    std::exit(1);
}

} // namespace Log
