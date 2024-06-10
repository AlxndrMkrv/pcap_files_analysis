#pragma once

#include "_Common.hxx"
#include <iostream>
#include <unordered_map>

namespace Statistics {

class L3Distribution {
public:
    enum class Category : size_t { IPV4, IPV6, OTHER };

    void update(const uint16_t etherType);

    size_t value(const Category c) const;

    friend std::ostream & operator<<(std::ostream & os,
                                     const L3Distribution & obj);

private:
    std::unordered_map<Category, size_t, EnumHash<Category>> _counters;
};

} // namespace Statistics
