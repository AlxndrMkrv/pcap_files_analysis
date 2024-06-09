#pragma once

#include "L2.hxx"

namespace Packet {

/*struct L2 {
    enum class FrameType { Unknown, Ethernet };

    L2(std::span<uint8_t> data, FrameType type) : _data(data), _frameType(type)
    {
    }

    virtual const L3 * const payload() const;

private:
    std::span<uint8_t> _data;
    FrameType _frameType = FrameType::Unknown;
};

struct ARP {};

struct IPv4 {};
struct IPv6 {};*/

struct Ethernet : public L2 {

    struct Mac {
        static constexpr size_t Size = 6;
        struct Compare {
            bool operator()(const Mac & lhs, const Mac & rhs)
            {
                return lhs < rhs;
            };
        };

        uint8_t operator[](const size_t idx) const { return octets[idx]; }
        auto operator<=>(const Mac & other) const = default;
        const uint8_t octets[Size];
    };

    uint16_t type() const { return _data[16]; }

    enum class Offset : int16_t {
        DESTINATION = 0,
        SOURCE = 6,
        ETHER_TYPE = 12,
        CRC = -4
    };

    static constexpr size_t MinimalSize = 64;
    static constexpr uint16_t VlanTpid = 0x8100;

    Ethernet(const std::span<const uint8_t> data);

protected:
    const Mac & destination;
    const Mac & source;
    /*protected:
        std::span<uint8_t, 6> _data;
        };

    protected:
        explicit Ethernet(std::span<const uint8_t> data) :
            L2(data), sourceMac{_data.data()}
        {
        }

        const uint8_t (&sourceMac)[6];
        const uint8_t (&destinationMac)[6];
        const uint16_t & type;
        const uint8_t (&checksum)[4];*/
};

} // namespace Packet
