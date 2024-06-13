#pragma once

#include "Base.hxx"
#include "IPv4.hxx"
#include "IPv6.hxx"

namespace TCP {

/*struct PseudoHeader {
    ::Packet::Address<4> Source;
    ::Packet::Address<4> Destination;
    uint16_t          : 8;
    uint16_t Protocol : 8;
    uint16_t Length   : 8;
};

struct PseudoHeader6 {
    ::Packet::Address<16> Source;
    ::Packet::Address<16> Destination;
    uint32_t Length;
    uint32_t          : 24;
    uint32_t Protocol : 8;
};*/

struct Header {
    uint16_t Source;
    uint16_t Destination;
    uint32_t Sequence;
    uint32_t AckNumber;
    /*union {
        struct {
            uint32_t     : 8;
            uint32_t CWR : 1;
            uint32_t ECE : 1;
            uint32_t URG : 1;
            uint32_t ACK : 1;
            uint32_t PSH : 1;
            uint32_t RST : 1;
            uint32_t SYN : 1;
            uint32_t FIN : 1;
            uint32_t     : 16;
        };
        struct {
            uint8_t DataOffset : 4;
            uint8_t            : 4;
            uint8_t            : 8;
            uint16_t WindowSize;
        };
    };*/
    uint8_t DataOffset : 4;
    uint8_t            : 4;
    union {
        struct {
            uint8_t CWR : 1;
            uint8_t ECE : 1;
            uint8_t URG : 1;
            uint8_t ACK : 1;
            uint8_t PSH : 1;
            uint8_t RST : 1;
            uint8_t SYN : 1;
            uint8_t FIN : 1;
        };
        uint8_t Flags;
    };
    uint16_t WindowSize;

    uint16_t Checksum;
};

class Packet : public ::Packet::Base<Header> {
    static_assert(sizeof(Header) == 5 * sizeof(uint32_t));

public:
    explicit Packet(const BytesSpan data, const IPv4::PseudoHeader phdr);
    explicit Packet(const BytesSpan data, const IPv6::PseudoHeader phdr);

    uint16_t source() const;
    uint16_t destination() const;
    uint8_t flags() const;

    /* ::Packet::Base<Header> implementation */
    bool isSane() const override;
    bool isChecksumOk() const override;
    BytesSpan payload() const override;
    void print(std::ostream & os) const override;

private:
    bool _checksumOk = false;
};

} // namespace TCP
