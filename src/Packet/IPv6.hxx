#pragma once

#include "Base.hxx"
#include <cstdint>

namespace IPv6 {

struct Address : ::Packet::Address<16> {};

struct Header {
    uint32_t Version       : 4;
    uint32_t TrafficClass  : 8;
    uint32_t FlowLabel     : 20;
    uint32_t PayloadLength : 16;
    uint32_t NextHeader    : 8;
    uint32_t HopLimit      : 8;
    Address Source;
    Address Destination;
};

class Packet : public ::Packet::Base<Header> {
    static_assert(sizeof(Header) == 40);

public:
    bool isSane() const override;
    bool isChecksumOk() const override;
    std::span<const uint8_t> payload() const override;

    uint8_t nextHeader() const;
    Address source() const;
    Address destination() const;
};

} // namespace IPv6
