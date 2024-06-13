#include "UDP.hxx"
#include "Checksum/RFC1071.hxx"
#include <bit>

namespace UDP {

Packet::Packet(const BytesSpan data, const IPv4::PseudoHeader & phdr) :
    Base<Header>(data)
{
    // Checksum may be unused in IPv4
    if (_header.Checksum == 0) {
        _checksumOk = true;
        return;
    }

    Checksum::RFC1071 cs;
    cs += BytesSpan(reinterpret_cast<const uint8_t *>(&phdr),
                    sizeof(IPv4::PseudoHeader));
    cs += data;
    _checksumOk = cs.isOk();
}

Packet::Packet(const BytesSpan data, const IPv6::PseudoHeader & phdr) :
    Base<Header>(data)
{
    Checksum::RFC1071 cs;
    cs += BytesSpan(reinterpret_cast<const uint8_t *>(&phdr),
                    sizeof(IPv6::PseudoHeader));
    cs += data;
    _checksumOk = cs.isOk();
}

uint16_t Packet::length() const { return std::byteswap(_header.Length); }

uint16_t Packet::source() const { return std::byteswap(_header.Source); }

uint16_t Packet::destination() const
{
    return std::byteswap(_header.Destination);
}

/* ************************************************************************** */
/*                    Base<Header> methods implementation                     */

bool Packet::isSane() const
{
    if (_data.size() < sizeof(Header) || _data.size() != length())
        return false;

    return true;
}

bool Packet::isChecksumOk() const { return _checksumOk; }

BytesSpan Packet::payload() const { return _data.subspan(sizeof(Header)); }

void Packet::print(std::ostream & os) const
{
    os << "UDP packet:" << std::endl;
    printBytes(os);
}

} // namespace UDP
