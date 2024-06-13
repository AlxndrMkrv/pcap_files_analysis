#include "L3Distribution.hxx"
#include "Packet/Ethernet.hxx"

namespace Statistics {

void L3Distribution::update(const uint16_t etherType)
{
    namespace EtherType = Ethernet::EtherType;

    const Category category = etherType == EtherType::IPV4   ? Category::IPV4
                              : etherType == EtherType::IPV6 ? Category::IPV6
                                                             : Category::OTHER;

    if (_counters.contains(category) &&
        _counters[category] == std::numeric_limits<size_t>::max())
        return;

    ++_counters[category];
}

size_t L3Distribution::value(const Category c) const
{
    if (!_counters.contains(c))
        return 0;

    return _counters.at(c);
}

std::ostream & operator<<(std::ostream & os, const L3Distribution & obj)
{
    using Cat = L3Distribution::Category;

    os << std::format("{}L3 packets distribution:", ItemIndent) << std::endl
       << std::format("{} IPv4: {}", SectionIndent, obj.value(Cat::IPV4))
       << std::endl
       << std::format("{} IPv6: {}", SectionIndent, obj.value(Cat::IPV6))
       << std::endl
       << std::format("{}other: {}", SectionIndent, obj.value(Cat::OTHER))
       << std::endl;

    return os;
}

} // namespace Statistics
