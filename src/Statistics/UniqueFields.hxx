#pragma once

#include "Log.hxx"
#include "Packet/Ethernet.hxx"
#include "Packet/IPv4.hxx"
#include "Packet/IPv6.hxx"
#include "Packet/TCP.hxx"
#include "Packet/UDP.hxx"
#include "_Common.hxx"
#include <cstddef>
#include <iostream>
#include <set>
#include <unordered_map>
#include <variant>

namespace Statistics {

class UniqueFields {
    using SetOfEthernetAddresses = std::set<::Ethernet::Address>;
    using SetOfIPv4Addresses = std::set<::IPv4::Address>;
    using SetOfIPv6Addresses = std::set<::IPv6::Address>;
    using SetOfPorts = std::set<uint16_t>;

    class Getter;

public:
    enum class Category : size_t {
        SOURCE_MAC,
        DESTINATION_MAC,
        SOURCE_IPV4,
        DESTINATION_IPV4,
        SOURCE_IPV6,
        DESTINATION_IPV6,
        SOURCE_PORT,
        DESTINATION_PORT
    };

    UniqueFields();

    void update(const Ethernet::Packet & packet);
    void update(const IPv4::Packet & packet);
    void update(const IPv6::Packet & packet);
    void update(const TCP::Packet & packet);
    void update(const UDP::Packet & packet);
    size_t value(const Category c) const;

    friend std::ostream & operator<<(std::ostream & os,
                                     const UniqueFields & obj);

private:
    // Getter getter();

    std::unordered_map<Category,
                       std::variant<SetOfEthernetAddresses, SetOfIPv4Addresses,
                                    SetOfIPv6Addresses, SetOfPorts>,
                       EnumHash<Category>>
        _counters;

    // template <typename TSet, class TValue>
    // void update(const Category cat, const TValue & val)
    // {
    //     if (!_counters.contains(cat))
    //         Log::Fatal(std::format("Container for category {} is
    //         uninitialized",
    //                                std::to_underlying(cat)));

    //     TSet * const set = std::get_if<TSet>(&_counters[cat]);

    //     if (set == nullptr) {
    //         Log::Fatal(std::format("Container for category {} is
    //         uninitialized",
    //                                std::to_underlying(cat)));
    //         return; // Suppress cppcheck warning
    //     }

    //     set->insert(val);
    // }

    // template <typename TSet>
    // size_t elementsNumber(const Category cat) const
    // {
    //     if (!_counters.contains(cat))
    //         Log::Fatal(std::format("Container for category {} is
    //         uninitialized",
    //                                std::to_underlying(cat)));

    //     const TSet * const set = std::get_if<TSet>(&_counters.at(cat));

    //     if (set == nullptr) {
    //         Log::Fatal(std::format("Container for category {} is
    //         uninitialized",
    //                                std::to_underlying(cat)));
    //         return 0; // Suppress cppcheck warning
    //     }

    //     return set->size();
    // }
};

} // namespace Statistics
