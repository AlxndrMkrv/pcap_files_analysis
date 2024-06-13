#include "ChecksumValidity.hxx"
#include "Statistics/_Common.hxx"

namespace Statistics {

void ChecksumValidity::update(const Packet::Abstract & packet)
{
    packet.isChecksumOk() ? ++_valids : ++_invalids;
}

size_t ChecksumValidity::valids() const { return _valids; }

size_t ChecksumValidity::invalids() const { return _invalids; }

size_t ChecksumValidity::total() const { return _valids + _invalids; }

std::ostream & operator<<(std::ostream & os, const ChecksumValidity & obj)
{
    os << std::format("{}  valid packets: {}", SectionIndent, obj.valids())
       << std::endl
       << std::format("{}invalid packets: {}", SectionIndent, obj.invalids())
       << std::endl;

    return os;
}

} // namespace Statistics
