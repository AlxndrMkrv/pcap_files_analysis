#include "BytesSpan.hxx"
#include <algorithm>

// BytesSpan::BytesSpan(const std::span<Underlying> span) :
//     ::std::span<Underlying>{span}
// {
// }

// BytesSpan::BytesSpan(const std::initializer_list<Underlying> bytes) :
//     ::std::span<Underlying>{bytes}
// {
// }

BytesSpan::BytesSpan(std::span<CByte> span) : ::std::span<CByte>{span} {}

BytesSpan::BytesSpan(const uint8_t dump[], const size_t size) :
    ::std::span<CByte>{static_cast<CByte *>(dump), size}
{
}

bool BytesSpan::operator==(const BytesSpan & other) const
{
    if (size() != other.size())
        return false;

    return std::equal(begin(), end(), other.begin());
}

/*BytesSpan BytesSpan::subspan(size_t offset, size_t size) const
{
    return ::std::span<CByte>::subspan(offset, size);
}*/
