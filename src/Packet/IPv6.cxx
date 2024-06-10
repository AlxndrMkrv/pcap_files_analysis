#include "IPv6.hxx"

namespace IPv6 {

bool Packet::isSane() const
{
    if (_data.size() < sizeof(Header) || _header.Version != 6 ||
        _data.size() != _header.PayloadLength + sizeof(Header))
        return false;

    return true;
}

bool Packet::isChecksumOk() const { return true; }

std::span<const uint8_t> Packet::payload() const
{
    return _data.subspan(sizeof(Header));
}

uint8_t Packet::nextHeader() const { return _header.NextHeader; }

Address Packet::source() const { return _header.Source; }

Address Packet::destination() const { return _header.Destination; }

} // namespace IPv6
