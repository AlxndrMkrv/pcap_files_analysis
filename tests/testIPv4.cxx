#include "Packet/IPv4.hxx"

bool GeneratePseudoHeader()
{
    {
        // FIN-ACK from 192.168.0.14:35452 to 62.212.64.16:461
        static constexpr uint8_t Dump[]{
            0x45, 0x00, 0x00, 0x34, 0xd8, 0xb8, 0x40, 0x00, 0x40, 0x06, 0x22,
            0x71, 0xc0, 0xa8, 0x00, 0x0e, 0x3e, 0xd4, 0x40, 0x10, 0x8a, 0x7c,
            0x01, 0xcd, 0x8f, 0xb0, 0xdc, 0xb2, 0x2b, 0x35, 0x38, 0x39, 0x80,
            0x11, 0x01, 0xf0, 0x3f, 0xc1, 0x00, 0x00, 0x01, 0x01, 0x08, 0x0a,
            0x4e, 0x90, 0xc8, 0x4c, 0xd7, 0xa7, 0x91, 0xe8};
        static constexpr uint8_t Source[]{0xc0, 0xa8, 0x00, 0x0e};
        static constexpr uint8_t Dest[]{0x3e, 0xd4, 0x40, 0x10};
        static constexpr uint8_t ValidPseudoHeader[]{0xc0, 0xa8, 0x00, 0x0e,
                                                     0x3e, 0xd4, 0x40, 0x10,
                                                     0x00, 0x06, 0x00, 0x20};
        const BytesSpan valid_phdr{ValidPseudoHeader,
                                   sizeof(ValidPseudoHeader)};

        const IPv4::Packet pkt{BytesSpan{Dump, sizeof(Dump)}};
        const IPv4::PseudoHeader pseudo = pkt.pseudoHeader();
        const BytesSpan pseudo_span{reinterpret_cast<const uint8_t *>(&pseudo),
                                    sizeof(IPv4::PseudoHeader)};

        if (!pkt.isSane())
            return false;

        if (!pkt.isChecksumOk())
            return false;

        if (pkt.totalLength() != sizeof(Dump))
            return false;

        if (pkt.protocol() != IPv4::Protocol::TCP)
            return false;

        if (pkt.source() != IPv4::Address{0xc0, 0xa8, 0x00, 0x0e})
            return false;

        if (pkt.destination() != IPv4::Address{0x3e, 0xd4, 0x40, 0x10})
            return false;

        for (auto i = 0; i < pseudo_span.size(); ++i)
            if (pseudo_span[i] != valid_phdr[i])
                return false;
    }

    return true;
}

bool testBroken()
{
    // packets failed to parse
    return true;
}

int main()
{
    if (!GeneratePseudoHeader())
        return 1;

    return 0;
}
