#pragma once

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
    std::unordered_map<
        Category,
        std::variant<std::set<Ethernet::Address>, std::set<IPv4::Address>,
                     std::set<IPv6::Address>, std::set<uint16_t>>,
        EnumHash<Category>>
        _counters;
};

} // namespace Statistics
