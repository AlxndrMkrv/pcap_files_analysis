#include "Packet/IPv4.hxx"
#include "Packet/IPv6.hxx"

bool GeneratePseudoHeader()
{
    {
        // ICMP from fe80::689a::20ff:fe37:e9ac to ff02::2
        static constexpr uint8_t Dump[]{
            0x60, 0x0a, 0xbf, 0x28, 0x00, 0x10, 0x3a, 0xff, 0xfe, 0x80,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x9a, 0x20, 0xff,
            0xfe, 0x37, 0xe9, 0xac, 0xff, 0x02, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
            0x85, 0x00, 0x96, 0x31, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
            0x6a, 0x9a, 0x20, 0x37, 0xe9, 0xac};
        static constexpr uint8_t Source[]{0xfe, 0x80, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x68, 0x9a, 0x20, 0xff,
                                          0xfe, 0x37, 0xe9, 0xac};
        static constexpr uint8_t Dest[]{0xff, 0x02, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x02};
        static constexpr uint8_t ValidPseudoHeader[]{
            0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x9a,
            0x20, 0xff, 0xfe, 0x37, 0xe9, 0xac, 0xff, 0x02, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x02, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x3a};

        // Check for dummy mistakes
        static_assert(sizeof(Source) == sizeof(IPv6::Address) &&
                      sizeof(Dest) == sizeof(IPv6::Address) &&
                      sizeof(ValidPseudoHeader) == sizeof(IPv6::PseudoHeader));

        const BytesSpan valid_phdr{ValidPseudoHeader,
                                   sizeof(ValidPseudoHeader)};
        const BytesSpan src_span{Source, sizeof(IPv6::Address)};
        const BytesSpan dst_span{Dest, sizeof(IPv6::Address)};

        const IPv6::Packet pkt{BytesSpan{Dump, sizeof(Dump)}};
        const IPv6::PseudoHeader pseudo = pkt.pseudoHeader();
        const BytesSpan pseudo_span{reinterpret_cast<const uint8_t *>(&pseudo),
                                    sizeof(IPv6::PseudoHeader)};

        if (!pkt.isSane())
            return false;

        if (!pkt.isChecksumOk())
            return false;

        if (pkt.payloadLength() != sizeof(Dump) - sizeof(IPv6::Header))
            return false;

        if (pkt.nextHeader() != IPv4::Protocol::ICMP6)
            return false;

        if (pkt.source() != src_span)
            return false;

        if (pkt.destination() != dst_span)
            return false;

        if (pseudo_span != valid_phdr)
            return false;
    }

    return true;
}

int main()
{
    if (!GeneratePseudoHeader())
        return 1;

    return 0;
}
