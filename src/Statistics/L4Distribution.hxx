#pragma once

#include "_Common.hxx"
#include <iostream>
#include <unordered_map>

namespace Statistics {

class L4Distribution {
public:
    enum class Category : size_t { TCP, UDP, ICMP, OTHER };

    void update(const uint8_t protocol);

    size_t value(const Category c) const;

    friend std::ostream & operator<<(std::ostream & os,
                                     const L4Distribution & obj);

private:
    std::unordered_map<Category, size_t, EnumHash<Category>> _counters;
};

} // namespace Statistics
