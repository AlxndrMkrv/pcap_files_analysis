#include "Ethernet.hxx"

namespace Ethernet {

Packet::Packet(const BytesSpan data) :
    Base<VlanHeader>(data),
    _checksum(*reinterpret_cast<const uint32_t *>(
        data.data() + (data.size() - sizeof(_checksum))))
{
}

uint16_t Packet::etherType() const
{
    return _header.EtherType == EtherType::VLAN
               ? std::byteswap(_header.VlanEtherType)
               : std::byteswap(_header.EtherType);
}

Address Packet::source() const { return _header.Source; }

Address Packet::destination() const { return _header.Destination; }

/* ************************************************************************** */
/*                    Base<Header> methods implementation                     */

bool Packet::isSane() const
{
    if ((etherType() == EtherType::VLAN && _data.size() < sizeof(VlanHeader)) ||
        _data.size() < sizeof(Header))
        return false;

    return true;
}

bool Packet::isChecksumOk() const { return true; }

BytesSpan Packet::payload() const
{
    if (etherType() == EtherType::VLAN) {
        const size_t size = _data.size() - sizeof(VlanHeader);
        return _data.subspan(sizeof(VlanHeader), size);
    }

    const size_t size = _data.size() - sizeof(Header);
    return _data.subspan(sizeof(Header), size);
}

void Packet::print(std::ostream & os) const
{
    os << "Ethernet packet:" << std::endl;
    printBytes(os);
}

} // namespace Ethernet
