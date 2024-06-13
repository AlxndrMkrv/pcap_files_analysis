#pragma once

#include "Base.hxx"
#include "IPv6.hxx"

namespace ICMP {

struct Header {
    uint8_t Type;
    uint8_t Code;
    uint16_t Checksum;
    uint32_t Rest;
};

class Packet : public ::Packet::Base<Header> {
    static_assert(sizeof(Header) == 2 * sizeof(uint32_t));

public:
    explicit Packet(const BytesSpan data);
    Packet(const BytesSpan data, const IPv6::PseudoHeader & phdr);

    /* ::Packet::Base<Header> implementation */
    bool isSane() const override;
    bool isChecksumOk() const override;
    BytesSpan payload() const override;
    void print(std::ostream & os) const override;

private:
    bool _checksumOk = false;
};

} // namespace ICMP
