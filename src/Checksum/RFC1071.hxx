#pragma once

#include "BytesSpan.hxx"

namespace Checksum {
class RFC1071 {
public:
    explicit RFC1071(const uint16_t initial = 0);
    // explicit RFC1071(BytesSpan data);
    explicit RFC1071(const BytesSpan & data);

    RFC1071 & operator+=(const BytesSpan & data);

    uint16_t big() const;
    uint16_t little() const;

    bool isOk() const;

private:
    size_t _sum = 0;
};
} // namespace Checksum
