#include "Ethernet.hxx"
#include <utility>

Packet::Ethernet::Ethernet(const std::span<const uint8_t> data) :
    L2(data), destination(*reinterpret_cast<const Mac *>(
                  &data[std::to_underlying(Offset::DESTINATION)])),
    source(*reinterpret_cast<const Mac *>(
        &data[std::to_underlying(Offset::SOURCE)]))
{
}
