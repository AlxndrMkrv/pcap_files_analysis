#include "UniqueFields.hxx"

namespace Statistics {

class UniqueFields::Getter {
public:
    template <typename TSet, class TValue>
    static void update(UniqueFields & obj, const Category cat,
                       const TValue & val)
    {
        if (!obj._counters.contains(cat))
            Log::Fatal(std::format("Container for category {} is uninitialized",
                                   std::to_underlying(cat)));

        TSet * const set = std::get_if<TSet>(&obj._counters[cat]);

        if (set == nullptr) {
            Log::Fatal(std::format("Container for category {} is uninitialized",
                                   std::to_underlying(cat)));
            return; // Suppress cppcheck warning
        }

        set->insert(val);
    }

    template <typename TSet>
    static size_t elementsNumber(const UniqueFields & obj, const Category cat)
    {
        if (!obj._counters.contains(cat))
            Log::Fatal(std::format("Container for category {} is uninitialized",
                                   std::to_underlying(cat)));

        const TSet * const set = std::get_if<TSet>(&obj._counters.at(cat));

        if (set == nullptr) {
            Log::Fatal(std::format("Container for category {} is uninitialized",
                                   std::to_underlying(cat)));
            return 0; // Suppress cppcheck warning
        }

        return set->size();
    }
};

UniqueFields::UniqueFields() :
    _counters{{Category::SOURCE_MAC, SetOfEthernetAddresses{}},
              {Category::DESTINATION_MAC, SetOfEthernetAddresses{}},
              {Category::SOURCE_IPV4, SetOfIPv4Addresses{}},
              {Category::DESTINATION_IPV4, SetOfIPv4Addresses{}},
              {Category::SOURCE_IPV6, SetOfIPv6Addresses{}},
              {Category::DESTINATION_IPV6, SetOfIPv6Addresses{}},
              {Category::SOURCE_PORT, SetOfPorts{}},
              {Category::DESTINATION_PORT, SetOfPorts{}}}
{
}

void UniqueFields::update(const Ethernet::Packet & packet)
{
    Getter::update<SetOfEthernetAddresses>(*this, Category::SOURCE_MAC,
                                           packet.source());
    Getter::update<SetOfEthernetAddresses>(*this, Category::DESTINATION_MAC,
                                           packet.destination());
}

void UniqueFields::update(const IPv4::Packet & packet)
{
    Getter::update<SetOfIPv4Addresses>(*this, Category::SOURCE_IPV4,
                                       packet.source());
    Getter::update<SetOfIPv4Addresses>(*this, Category::DESTINATION_IPV4,
                                       packet.destination());
}

void UniqueFields::update(const IPv6::Packet & packet)
{
    Getter::update<SetOfIPv6Addresses>(*this, Category::SOURCE_IPV6,
                                       packet.source());
    Getter::update<SetOfIPv6Addresses>(*this, Category::DESTINATION_IPV6,
                                       packet.destination());
}

void UniqueFields::update(const TCP::Packet & packet)
{
    Getter::update<SetOfPorts>(*this, Category::SOURCE_PORT, packet.source());
    Getter::update<SetOfPorts>(*this, Category::DESTINATION_PORT,
                               packet.destination());
}

void UniqueFields::update(const UDP::Packet & packet)
{
    Getter::update<SetOfPorts>(*this, Category::SOURCE_PORT, packet.source());
    Getter::update<SetOfPorts>(*this, Category::DESTINATION_PORT,
                               packet.destination());
}

size_t UniqueFields::value(const Category c) const
{
    switch (c) {
    case Category::SOURCE_MAC:
    case Category::DESTINATION_MAC:
        return Getter::elementsNumber<SetOfEthernetAddresses>(*this, c);
    case Category::SOURCE_IPV4:
    case Category::DESTINATION_IPV4:
        return Getter::elementsNumber<SetOfIPv4Addresses>(*this, c);
    case Category::SOURCE_IPV6:
    case Category::DESTINATION_IPV6:
        return Getter::elementsNumber<SetOfIPv6Addresses>(*this, c);
    case Category::SOURCE_PORT:
    case Category::DESTINATION_PORT:
        return Getter::elementsNumber<SetOfPorts>(*this, c);
    default:
        Log::Fatal(std::format("Unknown category {}", std::to_underlying(c)));
        return 0; // suppress warning
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
