#include "BytesSpan.hxx"
#include <algorithm>

BytesSpan::BytesSpan(std::span<Underlying> && span) :
    ::std::span<Underlying>{std::move(span)}
{
}

BytesSpan::BytesSpan(const std::span<Underlying> & span) :
    ::std::span<Underlying>{span}
{
}

bool BytesSpan::operator==(const BytesSpan & other) const
{
    if (size() != other.size())
        return false;

    return std::equal(begin(), end(), other.begin());
}
