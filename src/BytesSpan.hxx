#pragma once

#include <cstdint>
#include <span>

class BytesSpan : public std::span<const uint8_t> {
public:
    using Underlying = const uint8_t;

    using std::span<Underlying>::span;
    BytesSpan(std::span<Underlying> && span);

    bool operator==(const BytesSpan & other) const;

    // inline BytesSpan subspan(size_t offset, size_t size) const;
};
