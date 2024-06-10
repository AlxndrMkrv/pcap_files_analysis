#include "L4Distribution.hxx"
#include "Packet/IPv4.hxx"

namespace Statistics {

void L4Distribution::update(const uint8_t protocol)
{
    using Proto = Packet::IPv4::Protocol;

    const Category category =
        protocol == std::to_underlying(Proto::TCP)    ? Category::TCP
        : protocol == std::to_underlying(Proto::UDP)  ? Category::UDP
        : protocol == std::to_underlying(Proto::ICMP) ? Category::ICMP
                                                      : Category::OTHER;

    if (_counters.contains(category) &&
        _counters[category] == std::numeric_limits<size_t>::max())
        return;

    ++_counters[category];
}

size_t L4Distribution::value(const Category c) const
{
    if (!_counters.contains(c))
        return 0;

    return _counters.at(c);
}

std::ostream & operator<<(std::ostream & os, const L4Distribution & obj)
{
    using Cat = L4Distribution::Category;

    os << std::format("{}L4 packets distribution:", ItemIndent) << std::endl
       << std::format("{}        TCP: {}", SectionIndent, obj.value(Cat::TCP))
       << std::endl
       << std::format("{}        UDP: {}", SectionIndent, obj.value(Cat::UDP))
       << std::endl
       << std::format("{}ICMP/ICMPv6: {}", SectionIndent, obj.value(Cat::ICMP))
       << std::endl
       << std::format("{}      other: {}", SectionIndent, obj.value(Cat::OTHER))
       << std::endl;

    return os;
}

} // namespace Statistics
