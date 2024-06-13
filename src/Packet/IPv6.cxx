#include "IPv6.hxx"

namespace IPv6 {

Packet::Packet(const BytesSpan data) : ::Packet::Base<Header>(data) {}

const Header & Packet::header() const { return _header; }

PseudoHeader Packet::pseudoHeader() const
{
    const uint32_t length = payloadLength();
    return {_header.Source, _header.Destination, std::byteswap(length),
            _header.NextHeader};
}

uint8_t Packet::version() const { return (_data[0] & 0xF0) >> 4; }

uint8_t Packet::nextHeader() const { return _header.NextHeader; }

uint16_t Packet::payloadLength() const
{
    return std::byteswap(_header.PayloadLength);
}

Address Packet::source() const { return _header.Source; }

Address Packet::destination() const { return _header.Destination; }

/* ************************************************************************** */
/*                    Base<Header> methods implementation                     */

bool Packet::isSane() const
{
    if (_data.size() < sizeof(Header) || version() != 6 ||
        _data.size() < payloadLength() + sizeof(Header))
        return false;

    return true;
}

bool Packet::isChecksumOk() const { return true; }

BytesSpan Packet::payload() const
{
    return _data.subspan(sizeof(Header), payloadLength());
}

void Packet::print(std::ostream & os) const
{
    os << "IPv6 packet:" << std::endl;
    printBytes(os);
}

} // namespace IPv6
