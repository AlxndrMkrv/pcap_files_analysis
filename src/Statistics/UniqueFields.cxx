#include "UniqueFields.hxx"

namespace Statistics {

UniqueFields::UniqueFields() :
    _counters{{Category::SOURCE_MAC, std::set<Ethernet::Address>()},
              {Category::DESTINATION_MAC, std::set<Ethernet::Address>()},
              {Category::SOURCE_IPV4, std::set<IPv4::Address>()},
              {Category::DESTINATION_IPV4, std::set<IPv4::Address>()},
              {Category::SOURCE_IPV6, std::set<IPv6::Address>()},
              {Category::DESTINATION_IPV6, std::set<IPv6::Address>()},
              {Category::SOURCE_PORT, std::set<uint16_t>()},
              {Category::DESTINATION_PORT, std::set<uint16_t>()}}
{
}

void UniqueFields::update(const Ethernet::Packet & packet)
{
    std::get<std::set<Ethernet::Address>>(_counters.at(Category::SOURCE_MAC))
        .insert(packet.source());
    std::get<std::set<Ethernet::Address>>(
        _counters.at(Category::DESTINATION_MAC))
        .insert(packet.destination());
}

void UniqueFields::update(const IPv4::Packet & packet)
{
    std::get<std::set<IPv4::Address>>(_counters.at(Category::SOURCE_IPV4))
        .insert(packet.source());
    std::get<std::set<IPv4::Address>>(_counters.at(Category::DESTINATION_IPV4))
        .insert(packet.destination());
}

void UniqueFields::update(const IPv6::Packet & packet)
{
    std::get<std::set<IPv6::Address>>(_counters.at(Category::SOURCE_IPV6))
        .insert(packet.source());
    std::get<std::set<IPv6::Address>>(_counters.at(Category::DESTINATION_IPV6))
        .insert(packet.destination());
}

void UniqueFields::update(const TCP::Packet & packet)
{
    std::get<std::set<uint16_t>>(_counters.at(Category::SOURCE_PORT))
        .insert(packet.source());
    std::get<std::set<uint16_t>>(_counters.at(Category::DESTINATION_PORT))
        .insert(packet.destination());
}

void UniqueFields::update(const UDP::Packet & packet)
{
    std::get<std::set<uint16_t>>(_counters.at(Category::SOURCE_PORT))
        .insert(packet.source());
    std::get<std::set<uint16_t>>(_counters.at(Category::DESTINATION_PORT))
        .insert(packet.destination());
}

size_t UniqueFields::value(const Category c) const
{
    if (!_counters.contains(c))
        return 0;

    switch (c) {
    case Category::SOURCE_MAC:
    case Category::DESTINATION_MAC:
        return std::get<std::set<Ethernet::Address>>(_counters.at(c)).size();
    case Category::SOURCE_IPV4:
    case Category::DESTINATION_IPV4:
        return std::get<std::set<IPv4::Address>>(_counters.at(c)).size();
    case Category::SOURCE_IPV6:
    case Category::DESTINATION_IPV6:
        return std::get<std::set<IPv6::Address>>(_counters.at(c)).size();
    case Category::SOURCE_PORT:
    case Category::DESTINATION_PORT:
        return std::get<std::set<uint16_t>>(_counters.at(c)).size();
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
                      obj.value(Cat::SOURCE_IPV4) + obj.value(Cat::SOURCE_IPV6))
       << std::endl
       << std::format("{}    Dest IP: {}", SectionIndent,
                      obj.value(Cat::DESTINATION_IPV4) +
                          obj.value(Cat::DESTINATION_IPV6))
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
