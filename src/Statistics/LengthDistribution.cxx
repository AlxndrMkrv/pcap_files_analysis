#include "LengthDistribution.hxx"
#include <format>

namespace Statistics {

void LengthDistribution::update(const size_t value)
{
    // NOLINTBEGIN(readability-avoid-nested-conditional-operator)
    const Category category = value <= 64     ? Category::LEQ_64
                              : value <= 255  ? Category::BW_65_255
                              : value <= 511  ? Category::BW_256_511
                              : value <= 1023 ? Category::BW_512_1023
                              : value <= 1518 ? Category::BW_1024_1518
                                              : Category::GEQ_1519;
    // NOLINTEND(readability-avoid-nested-conditional-operator)

    if (_counters.contains(category) &&
        _counters[category] == std::numeric_limits<size_t>::max())
        return;

    ++_counters[category];
}

size_t LengthDistribution::value(const Category c) const
{
    if (!_counters.contains(c))
        return 0;

    return _counters.at(c);
}

std::ostream & operator<<(std::ostream & os, const LengthDistribution & obj)
{
    using Cat = LengthDistribution::Category;

    os << std::format("{}Packets size distribution:", ItemIndent) << std::endl
       << std::format("{}    <= 64  : {}", SectionIndent,
                      obj.value(Cat::LEQ_64))
       << std::endl
       << std::format("{}  65 - 255 : {}", SectionIndent,
                      obj.value(Cat::BW_65_255))
       << std::endl
       << std::format("{} 256 - 511 : {}", SectionIndent,
                      obj.value(Cat::BW_256_511))
       << std::endl
       << std::format("{} 512 - 1023: {}", SectionIndent,
                      obj.value(Cat::BW_512_1023))
       << std::endl
       << std::format("{}1024 - 1518: {}", SectionIndent,
                      obj.value(Cat::BW_1024_1518))
       << std::endl
       << std::format("{}    >= 1519: {}", SectionIndent,
                      obj.value(Cat::GEQ_1519))
       << std::endl;

    return os;
}

} // namespace Statistics
