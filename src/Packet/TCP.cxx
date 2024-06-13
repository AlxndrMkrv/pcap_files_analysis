#include "TCP.hxx"
#include "Checksum/RFC1071.hxx"

namespace TCP {

Packet::Packet(const BytesSpan data, const IPv4::PseudoHeader phdr) :
    ::Packet::Base<Header>(data)
{
    Checksum::RFC1071 cs;
    cs += BytesSpan(reinterpret_cast<const uint8_t *>(&phdr),
                    sizeof(IPv4::PseudoHeader));
    cs += data;
    _checksumOk = cs.isOk();
}

Packet::Packet(const BytesSpan data, const IPv6::PseudoHeader phdr) :
    ::Packet::Base<Header>(data)
{
    Checksum::RFC1071 cs;
    cs += BytesSpan(reinterpret_cast<const uint8_t *>(&phdr),
                    sizeof(IPv6::PseudoHeader));
    cs += data;
    _checksumOk = cs.isOk();
}

uint16_t Packet::source() const { return std::byteswap(_header.Source); }

uint16_t Packet::destination() const
{
    return std::byteswap(_header.Destination);
}

uint8_t Packet::flags() const { return _header.Flags; }

/* ************************************************************************** */
/*                    Base<Header> methods implementation                     */

bool Packet::isSane() const
{
    if (_data.size() < sizeof(Header))
        return false;

    return true;
}

bool Packet::isChecksumOk() const { return _checksumOk; }

BytesSpan Packet::payload() const { return _data.subspan(0, 1); }

void Packet::print(std::ostream & os) const
{
    os << "TCP packet:" << std::endl;
    printBytes(os);
}

} // namespace TCP
