#include "IPv4.hxx"

namespace IPv4 {

bool Packet::isSane() const
{
    if (_data.size() < sizeof(Header) || _data.size() < _header.IHL ||
        _header.TotalLength < sizeof(Header) ||
        _data.size() != _header.TotalLength)
        return false;

    return true;
}

bool Packet::isChecksumOk() const
{
    return ::Packet::VerifyChecksum1071(_data.subspan(0, _header.IHL),
                                        _header.HeaderChecksum);
}

std::span<const uint8_t> Packet::payload() const
{
    const uint8_t headerLength = _header.IHL;
    return _data.subspan(headerLength, _data.size() - headerLength);
}

uint8_t Packet::protocol() const { return _header.Protocol; }

Address Packet::source() const { return _header.Source; }

Address Packet::destination() const { return _header.Destination; }

} // namespace IPv4
