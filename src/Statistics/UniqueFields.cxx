#include "UniqueFields.hxx"

namespace Statistics {

size_t UniqueFields::value(const Category c) const
{
    if (!_counters.contains(c))
        return 0;

    switch (c) {
    case Category::SOURCE_MAC:
    case Category::DESTINATION_MAC:
        return std::get<std::set<Packet::Ethernet::Mac>>(_counters.at(c))
            .size();
    default:
        return 0;
    }
}

std::ostream & operator<<(std::ostream & os, const UniqueFields & obj)
{
    using Cat = UniqueFields::Category;

    os << std::format("{}Unique fields number:", ItemIndent) << std::endl
       << std::format("{} Source MAC: {}", SectionIndent,
                      obj.value(Cat::SOURCE_MAC))
       << std::endl
       << std::format("{}   Dest MAC: {}", SectionIndent,
                      obj.value(Cat::DESTINATION_MAC))
       << std::endl
       << std::format("{}  Source IP: {}", SectionIndent,
                      obj.value(Cat::SOURCE_IP4) + obj.value(Cat::SOURCE_IP6))
       << std::endl
       << std::format("{}    Dest IP: {}", SectionIndent,
                      obj.value(Cat::DESTINATION_IP4) +
                          obj.value(Cat::DESTINATION_IP6))
       << std::endl
       << std::format("{}Source Port: {}", SectionIndent,
                      obj.value(Cat::SOURCE_PORT))
       << std::endl
       << std::format("{}  Dest Port: {}", SectionIndent,
                      obj.value(Cat::DESTINATION_PORT))
       << std::endl;

    return os;
}

} // namespace Statistics
