#pragma once

#include "Base.hxx"

namespace Ethernet {

struct Address : ::Packet::Address<6> {};

struct Header {
    Address Destination;
    Address Source;
    uint16_t EtherType;
};

struct VlanHeader : public Header {
    uint16_t : 16;
    uint16_t VlanEtherType;
};

namespace EtherType {
enum Type {
    IPV4 = 0x0800,
    VLAN = 0x8100,
    IPV6 = 0x86DD,
};
} // namespace EtherType

class Packet : public ::Packet::Base<VlanHeader> {
    static_assert(sizeof(Header) == 14 && sizeof(VlanHeader) == 18);

public:
    explicit Packet(const BytesSpan data);

    uint16_t etherType() const;
    Address source() const;
    Address destination() const;

    /* ::Packet::Base<Header> implementation */
    bool isSane() const override;
    bool isChecksumOk() const override;
    BytesSpan payload() const override;
    void print(std::ostream & os) const override;

protected:
    const uint32_t & _checksum;
};

} // namespace Ethernet
