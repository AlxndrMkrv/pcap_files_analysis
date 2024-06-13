#include <format>
#include <iostream>
#include <source_location>
#include <string_view>

void Error(std::string_view msg,
           std::source_location loc = std::source_location::current())
{
    std::cerr << std::format("{}:{}: {}", loc.file_name(), loc.line(), msg);
}

void Fatal(std::string_view msg,
           std::source_location loc = std::source_location::current())
{
    Error(msg, loc);
    std::exit(1);
}
