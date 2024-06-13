#include "IPv4.hxx"
#include "Checksum/RFC1071.hxx"

namespace IPv4 {

Packet::Packet(const BytesSpan data) : Base<Header>(data) {}

const Header & Packet::header() const { return _header; }

PseudoHeader Packet::pseudoHeader() const
{
    const uint16_t length = totalLength() - headerSize();
    return {_header.Source, _header.Destination, _header.Protocol,
            std::byteswap(length)};
}

uint8_t Packet::version() const { return (_data[0] & 0xF0) >> 4; }

uint16_t Packet::headerSize() const
{
    return (_data[0] & 0x0F) * sizeof(uint32_t);
}

uint16_t Packet::totalLength() const
{
    return std::byteswap(_header.TotalLength);
}

uint8_t Packet::protocol() const { return _header.Protocol; }

Address Packet::source() const { return _header.Source; }

Address Packet::destination() const { return _header.Destination; }

/* ************************************************************************** */
/*                    Base<Header> methods implementation                     */

bool Packet::isSane() const
{
    const size_t tl = totalLength();
    const size_t hs = headerSize();
    const size_t ver = version();

    if (_data.size() < sizeof(Header) || _data.size() < headerSize() ||
        _data.size() < totalLength() || version() != 4)
        return false;

    return true;
}

bool Packet::isChecksumOk() const
{
    return Checksum::RFC1071(_data.subspan(0, headerSize())).isOk();
}

BytesSpan Packet::payload() const
{
    return _data.subspan(headerSize(), totalLength() - headerSize());
}

void IPv4::Packet::print(std::ostream & os) const
{
    os << "IPv4 packet:" << std::endl;
    printBytes(os);
}

} // namespace IPv4
