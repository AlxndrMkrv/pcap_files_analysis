#pragma once

#include "Base.hxx"
#include <cstdint>

namespace IPv6 {

struct Address : ::Packet::Address<16> {};

struct Header {
    uint32_t Version       : 4;
    uint32_t TrafficClass  : 8;
    uint32_t FlowLabel     : 20;
    uint16_t PayloadLength;
    uint8_t NextHeader;
    uint8_t HopLimit;
    Address Source;
    Address Destination;
};

// Header to pass to TCP/UDP/ICMP packets
struct PseudoHeader {
    Address Source;
    Address Destination;
    uint32_t Length;
    uint32_t          : 24;
    uint32_t Protocol : 8;
};

class Packet : public ::Packet::Base<Header> {
    static_assert(sizeof(Header) == 10 * sizeof(uint32_t) &&
                  sizeof(PseudoHeader) == 10 * sizeof(uint32_t));

public:
    explicit Packet(const BytesSpan);

    const Header & header() const;
    PseudoHeader pseudoHeader() const;
    uint8_t version() const;
    uint8_t nextHeader() const;
    uint16_t payloadLength() const;
    Address source() const;
    Address destination() const;

    /* ::Packet::Base<Header> implementation */
    bool isSane() const override;
    bool isChecksumOk() const override;
    BytesSpan payload() const override;
    void print(std::ostream & os) const override;
};

} // namespace IPv6
