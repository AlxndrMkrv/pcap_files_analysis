#pragma once

#include <cstdint>
#include <span>

namespace Packet {

template <class Header>
struct Base {
public:
    virtual bool isSane() const = 0;
    virtual bool isChecksumOk() const = 0;
    virtual std::span<const uint8_t> payload() const = 0;

protected:
    explicit Base(const std::span<const uint8_t> data);

    const std::span<const uint8_t> _data;
    const Header & _header;
};

template <size_t _Size>
struct Address {
    static constexpr size_t Size() { return _Size; }

    struct Compare {
        bool operator()(const Address & lhs, const Address & rhs)
        {
            return lhs < rhs;
        };
    };

    auto operator<=>(const Address & other) const = default;

    uint8_t octets[_Size];
};

uint16_t Checksum1071(std::span<const uint8_t> data,
                      const uint16_t checksum = 0);
bool VerifyChecksum1071(std::span<const uint8_t> data, const uint16_t checksum);

uint32_t Crc32(std::span<const uint8_t> data);

extern const std::array<uint32_t, 256> Crc32Table;

} // namespace Packet
