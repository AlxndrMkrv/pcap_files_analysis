#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

namespace Statistics {

static constexpr const char * ItemIndent = "    ";
static constexpr const char * SectionIndent = "        ";

template <typename T>
concept EnumClass =
    std::is_enum_v<T> && !std::is_convertible_v<T, std::underlying_type_t<T>>;

template <EnumClass T>
struct EnumHash {
    size_t operator()(const T & key) const { return std::to_underlying(key); }
};

} // namespace Statistics
