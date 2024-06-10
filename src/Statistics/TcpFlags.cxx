#include "TcpFlags.hxx"

namespace Statistics {

namespace {
namespace Masks {
enum {
    SYN = 0b01000000,
    SYN_ACK = 0b01001000,
    ACK = 0b00001000,
    FIN_ACK = 0b10001000,
    RST = 0b00100000,
    RST_ACK = 0b001010000
};
} // namespace Masks
} // namespace

void TcpFlags::update(const uint8_t flags)
{
    const Category category = (flags == Masks::SYN)       ? Category::SYN
                              : (flags == Masks::SYN_ACK) ? Category::SYN_ACK
                              : (flags == Masks::ACK)     ? Category::ACK
                              : (flags == Masks::FIN_ACK) ? Category::FIN_ACK
                              : (flags == Masks::RST)     ? Category::RST
                              : (flags == Masks::RST_ACK) ? Category::RST_ACK
                                                          : Category::OTHER;

    if (_counters.contains(category) &&
        _counters[category] == std::numeric_limits<size_t>::max())
        return;

    ++_counters[category];
}

size_t TcpFlags::value(const Category c) const
{
    if (!_counters.contains(c))
        return 0;

    return _counters.at(c);
}

std::ostream & operator<<(std::ostream & os, const TcpFlags & obj)
{
    using Cat = TcpFlags::Category;

    os << std::format("{}TCP packet flags distribution:", ItemIndent)
       << std::endl
       << std::format("{}    SYN: {}", SectionIndent, obj.value(Cat::SYN))
       << std::endl
       << std::format("{}SYN+ACK: {}", SectionIndent, obj.value(Cat::SYN_ACK))
       << std::endl
       << std::format("{}    ACK: {}", SectionIndent, obj.value(Cat::ACK))
       << std::endl
       << std::format("{}FIN+ACK: {}", SectionIndent, obj.value(Cat::FIN_ACK))
       << std::endl
       << std::format("{}    RST: {}", SectionIndent, obj.value(Cat::RST))
       << std::endl
       << std::format("{}RST+ACK: {}", SectionIndent, obj.value(Cat::RST_ACK))
       << std::endl
       << std::format("{}  other: {}", SectionIndent, obj.value(Cat::OTHER))
       << std::endl;

    return os;
}

} // namespace Statistics
