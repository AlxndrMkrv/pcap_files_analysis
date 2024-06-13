#include "ICMP.hxx"
#include <Checksum/RFC1071.hxx>

namespace ICMP {

Packet::Packet(const BytesSpan data) : ::Packet::Base<Header>(data)
{
    _checksumOk = Checksum::RFC1071{data}.isOk();
}

Packet::Packet(const BytesSpan data, const IPv6::PseudoHeader & phdr) :
    ::Packet::Base<Header>(data)
{
    Checksum::RFC1071 cs;
    cs += BytesSpan(reinterpret_cast<const uint8_t *>(&phdr),
                    sizeof(IPv6::PseudoHeader));
    cs += data;
    _checksumOk = cs.isOk();
}

bool Packet::isSane() const
{
    if (_data.size() < sizeof(Header))
        return false;

    return true;
}

bool Packet::isChecksumOk() const { return _checksumOk; }

BytesSpan Packet::payload() const
{
    return _data.subspan(sizeof(Header), _data.size() - sizeof(Header));
}

void Packet::print(std::ostream & os) const
{
    os << "ICMP packet:" << std::endl;
    printBytes(os);
}

} // namespace ICMP
