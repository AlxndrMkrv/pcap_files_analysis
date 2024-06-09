#pragma once

#include "_Common.hxx"
#include <cstddef>
#include <iostream>
#include <print>
#include <unordered_map>

namespace Statistics {

class LengthDistribution {
public:
    enum class Category : size_t {
        LEQ_64,
        BW_65_255,
        BW_256_511,
        BW_512_1023,
        BW_1024_1518,
        GEQ_1519
    };

    void update(std::size_t value);

    size_t value(const Category c) const;

    friend std::ostream & operator<<(std::ostream & os,
                                     const LengthDistribution & obj);

private:
    /* It's more efficient to use
     * std::array<size_t, static_cast<size_t>(Category::__SIZE)> _counter;
     * here. But methods implementation will be less clear and clang-tidy yells
     * on them like insane
     */
    std::unordered_map<Category, size_t, EnumHash<Category>> _counters;
};

} // namespace Statistics
