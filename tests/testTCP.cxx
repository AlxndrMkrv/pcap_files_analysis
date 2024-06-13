#include "Packet/TCP.hxx"

bool FromIPv4()
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

        const IPv4::Packet ipv4{BytesSpan{Dump, sizeof(Dump)}};

        // refer to IPv4 tests if something goes wrong here
        if (!ipv4.isSane() || !ipv4.isChecksumOk() ||
            ipv4.protocol() != IPv4::Protocol::TCP)
            return false;

        TCP::Packet tcp{ipv4.payload(), ipv4.pseudoHeader()};

        if (!tcp.isSane())
            return false;

        if (!tcp.isChecksumOk())
            return false;

        if (tcp.source() != 35452)
            return false;

        if (tcp.destination() != 461)
            return false;

        if (tcp.flags() != 0b01001000)
            return false;
    }

    return true;
}

int main()
{
    if (FromIPv4())
        return 1;

    return 0;
}
