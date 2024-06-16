#pragma once

#include "_Common.hxx"
#include <ostream>
#include <unordered_map>

namespace Statistics {

class TcpFlags {
public:
    enum class Category : size_t {
        SYN,
        SYN_ACK,
        ACK,
        FIN_ACK,
        RST,
        RST_ACK,
        OTHER
    };

    void update(const uint8_t flags);

    size_t value(const Category c) const;

    friend std::ostream & operator<<(std::ostream & os, const TcpFlags & obj);

private:
    std::unordered_map<Category, size_t, EnumHash<Category>> _counters;
};

} // namespace Statistics
