#pragma once

#include "Base.hxx"
#include "IPv4.hxx"
#include "IPv6.hxx"

namespace UDP {

struct Header {
    uint16_t Source;
    uint16_t Destination;
    uint16_t Length;
    uint16_t Checksum;
};

class Packet : public ::Packet::Base<Header> {
    static_assert(sizeof(Header) == 8);

public:
    Packet(const BytesSpan data, const IPv4::PseudoHeader & phdr);
    Packet(const BytesSpan data, const IPv6::PseudoHeader & phdr);

    uint16_t length() const;
    uint16_t source() const;
    uint16_t destination() const;

    /* ::Packet::Base<Header> implementation */
    bool isSane() const override;
    bool isChecksumOk() const override;
    BytesSpan payload() const override;
    void print(std::ostream & os) const override;

private:
    bool _checksumOk = false;
};

} // namespace UDP
