#include "Ethernet.hxx"
#include <utility>

namespace Ethernet {

Packet::Packet(const std::span<const uint8_t> data) :
    Base<VlanHeader>(data),
    _checksum(*reinterpret_cast<const uint32_t *>(
        data.data() + (data.size() - sizeof(_checksum))))
{
}

bool Packet::isSane() const
{
    if (_data.size() < MinimalSize)
        return false;

    return true;
}

bool Packet::isChecksumOk() const
{
    return ::Packet::Crc32(_data.subspan(0, _data.size() - 4)) == _checksum;
}

std::span<const uint8_t> Packet::payload() const
{
    if (_header.EtherType == std::to_underlying(EtherType::VLAN)) {
        const size_t size =
            _data.size() - sizeof(VlanHeader) - sizeof(_checksum);
        return _data.subspan(sizeof(VlanHeader), size);
    }

    const size_t size = _data.size() - sizeof(Header) - sizeof(_checksum);
    return _data.subspan(sizeof(Header), size);
}

uint16_t Packet::etherType() const
{
    return _header.EtherType == std::to_underlying(EtherType::VLAN)
               ? _header.VlanEtherType
               : _header.EtherType;
}

Address Packet::source() const { return _header.Source; }

Address Packet::destination() const { return _header.Destination; }

} // namespace Ethernet
