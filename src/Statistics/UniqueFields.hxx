#pragma once

#include "Packet/Ethernet.hxx"
#include "_Common.hxx"
#include <cstddef>
#include <iostream>
#include <set>
#include <unordered_map>
#include <variant>

namespace Statistics {

class UniqueFields {
public:
    /*struct ISet {
        virtual void
    }

    template <class T>
    struct Set : public std::set<T> {
        size_t value
    };*/

    enum class Category : size_t {
        SOURCE_MAC,
        DESTINATION_MAC,
        SOURCE_IP4,
        DESTINATION_IP4,
        SOURCE_IP6,
        DESTINATION_IP6,
        SOURCE_PORT,
        DESTINATION_PORT
    };

    void update(const Packet::Ethernet::Mac & sourceMac,
                const Packet::Ethernet::Mac & destMac);
    size_t value(const Category c) const;

    friend std::ostream & operator<<(std::ostream & os,
                                     const UniqueFields & obj);

private:
    // template <class T>
    // auto & getSet(const Category c);

    std::unordered_map<Category, std::variant<std::set<Packet::Ethernet::Mac>>,
                       EnumHash<Category>>
        _counters;
};

} // namespace Statistics
