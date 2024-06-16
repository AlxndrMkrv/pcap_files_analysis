#include "TcpFlags.hxx"
#include <cstddef>
#include <cstdint>
#include <format>
#include <limits>

namespace Statistics {

namespace {
namespace Masks {
enum : uint8_t {
    // Dirty workaround
    SYN = 0b00000010,     // 0b01000000,
    SYN_ACK = 0b00010010, // 0b01001000,
    ACK = 0b00010000,     // 0b00001000,
    FIN_ACK = 0b00010001, // 0b10001000,
    RST = 0b00000100,     // 0b00100000,
    RST_ACK = 0b000010100 // 0b001010000
};
} // namespace Masks

constexpr TcpFlags::Category flag2category(const uint8_t flags)
{
    using Cat = TcpFlags::Category;

    switch (flags) {
    case Masks::SYN:
        return Cat::SYN;
    case Masks::SYN_ACK:
        return Cat::SYN_ACK;
    case Masks::ACK:
        return Cat::ACK;
    case Masks::FIN_ACK:
        return Cat::FIN_ACK;
    case Masks::RST:
        return Cat::RST;
    case Masks::RST_ACK:
        return Cat::RST_ACK;
    default:
        return Cat::OTHER;
    }
}

} // namespace

void TcpFlags::update(const uint8_t flags)
{
    const Category category = flag2category(flags);

    if (_counters[category] == std::numeric_limits<size_t>::max())
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

    os << std::format("{}TCP packet flags distribution:\n", ItemIndent)
       << std::format("{}    SYN: {}\n", SectionIndent, obj.value(Cat::SYN))
       << std::format("{}SYN+ACK: {}\n", SectionIndent, obj.value(Cat::SYN_ACK))
       << std::format("{}    ACK: {}\n", SectionIndent, obj.value(Cat::ACK))
       << std::format("{}FIN+ACK: {}\n", SectionIndent, obj.value(Cat::FIN_ACK))
       << std::format("{}    RST: {}\n", SectionIndent, obj.value(Cat::RST))
       << std::format("{}RST+ACK: {}\n", SectionIndent, obj.value(Cat::RST_ACK))
       << std::format("{}  other: {}\n", SectionIndent, obj.value(Cat::OTHER));

    return os;
}

} // namespace Statistics
