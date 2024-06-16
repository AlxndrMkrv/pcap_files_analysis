#pragma once

#include "Base.hxx"
#include <cstdint>

namespace IPv4 {

struct Address : ::Packet::Address<4> {
    using ::Packet::Address<4>::Address;
};

struct Header {
    uint8_t Version : 4;
    uint8_t IHL     : 4;
    uint8_t         : 8;
    uint16_t TotalLength;
    uint16_t Identification;
    uint16_t Flags          : 3;
    uint16_t FragmentOffset : 13;
    uint8_t TimeToLive;
    uint8_t Protocol;
    uint16_t HeaderChecksum;
    Address Source;
    Address Destination;
};

// Header to pass to TCP/UDP packets
struct PseudoHeader {
    Address Source;
    Address Destination;
    uint16_t          : 8;
    uint16_t Protocol : 8;
    uint16_t Length;
};

namespace Protocol {
enum : uint8_t { ICMP = 0x01, TCP = 0x06, UDP = 0x11, ICMP6 = 0x3A };
} // namespace Protocol

class Packet : public ::Packet::Base<Header> {
    static_assert(sizeof(Header) == 5 * sizeof(uint32_t) &&
                  sizeof(PseudoHeader) == 3 * sizeof(uint32_t));

public:
    explicit Packet(const BytesSpan data);

    const Header & header() const;
    PseudoHeader pseudoHeader() const;
    uint8_t version() const;
    uint16_t headerSize() const;
    uint16_t totalLength() const;
    uint8_t protocol() const;
    Address source() const;
    Address destination() const;

    /* ::Packet::Base<Header> implementation */
    bool isSane() const override;
    bool isChecksumOk() const override;
    BytesSpan payload() const override;
    void print(std::ostream & os) const override;
};

} // namespace IPv4
