#pragma once

#include "Base.hxx"
#include <cstdint>

namespace IPv4 {

struct Address : ::Packet::Address<4> {};

struct Header {
    uint32_t Version        : 4;
    uint32_t IHL            : 4;
    uint32_t                : 8;
    uint32_t TotalLength    : 16;
    uint32_t Identification : 16;
    uint32_t Flags          : 3;
    uint32_t FragmentOffset : 13;
    uint8_t TimeToLive;
    uint8_t Protocol;
    uint16_t HeaderChecksum;
    Address Source;
    Address Destination;
};

namespace Protocol {
enum { ICMP = 1, TCP = 16, UDP = 17 };
}

class Packet : public ::Packet::Base<Header> {
    static_assert(sizeof(Header) == 20);

public:
    using Base<Header>::Base;

    bool isSane() const override;
    bool isChecksumOk() const override;
    std::span<const uint8_t> payload() const override;

    uint8_t protocol() const;
    Address source() const;
    Address destination() const;
};

} // namespace IPv4
