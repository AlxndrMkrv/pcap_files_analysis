#include "BytesSpan.hxx"

BytesSpan::BytesSpan(std::span<Underlying> && span) :
    ::std::span<Underlying>(span)
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
    return std::span<Underlying>::subspan(offset, size);
}*/
