#include "RFC1071.hxx"
#include <limits>
#include <numeric>

namespace Checksum {

RFC1071::RFC1071(const uint16_t initial) : _sum(initial) {}

// RFC1071::RFC1071(BytesSpan data) { operator+=(data); }

RFC1071::RFC1071(const BytesSpan & data) { operator+=(data); }

RFC1071 & RFC1071::operator+=(const BytesSpan & data)
{
    /*
     * It appears that there is no problem calculating a big-endian checksum on
     * a little-endian CPU
     */

    // Reinterpret data as span of 16-bit words
    const std::span<const uint16_t> wordSpan(
        reinterpret_cast<const uint16_t *>(data.data()),
        data.size() / sizeof(uint16_t));

    // Sum up all the words one by one
    _sum = std::accumulate(wordSpan.begin(), wordSpan.end(), _sum);

    // Add the last byte if the length is odd
    if (data.size() % sizeof(uint16_t) == 1)
        _sum += data.back();

    // Fold the sum to an uint16_t number
    while (static_cast<bool>(_sum >> std::numeric_limits<uint16_t>::digits))
        _sum = (_sum & std::numeric_limits<uint16_t>::max()) +
               (_sum >> std::numeric_limits<uint16_t>::digits);

    return *this;
}

uint16_t RFC1071::big() const
{
    // Leave the sum as is to compare with checksums in headers
    return _sum;
}

uint16_t RFC1071::little() const
{
    // Swap the bytes to convert the checksum into a little-endian number
    return std::byteswap(static_cast<uint16_t>(_sum));
}

bool RFC1071::isOk() const
{
    return _sum == std::numeric_limits<uint16_t>::max();
}

} // namespace Checksum
