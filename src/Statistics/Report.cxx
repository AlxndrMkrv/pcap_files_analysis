#include "Report.hxx"
#include "Packet/Ethernet.hxx"
#include "_Common.hxx"
#include <format>

namespace Statistics {

Report::Report(const std::filesystem::path & filename) : _filename(filename) {}

void Report::markInvalid() { _fileInvalid = true; }

const std::string & Report::filename() const { return _filename; }

void Report::analysePacket(const std::span<const uint8_t> data)
{
    ++_nPackets;
    _nBytes += data.size();

    _lengthDistribution.update(data.size());

    Ethernet::Packet eth{data};

    if (!pkt.isSane())
        return;

    _l3Distribution.update(eth.etherType());

    switch (eth.etherType()) {
    case Ethernet::EtherType::IPV4:
        analyseIPv4(eth.payload());
        break;
    case Ethernet::EtherType::IPV6:
        analyseIPv6(eth.payload());
        break;
    default:
    }
}

void Report::analyseIPv4(const std::span<const uint8_t> data)
{
    IPv4::Packet pkt{data};

    if (!pkt.isSane())
        return;

    switch (pkt.protocol())
    case IPv4::Protocol::TCP:
        analyseTcp(pkt.payload());
    break;
case IPv4::Protocol::UDP:
    analyseUdp(pkt.payload());
    break;
case IPv4::Protocol::ICMP:
    analyseIcmp(pkt.payload());
    break;
}

void Report::analyseIPv6(const std::span<const uint8_t> data) {}

void Report::analyseICMP(const std::span<const uint8_t> data) {}

void Report::analyseTCP(const std::span<const uint8_t> data) {}

void Report::analyseUDP(const std::span<const uint8_t> data) {}

std::ostream & operator<<(std::ostream & os, const Report & obj)
{
    if (obj._fileInvalid) {
        os << "File \"" << obj._filename << "\" parse failed" << std::endl;
        return os;
    }

    os << std::format("File \"{}\":", obj._filename) << std::endl
       << std::format("{}Number of packets: {}", ItemIndent, obj._nPackets)
       << std::endl
       << std::format("{}Total number of bytes: {}", ItemIndent, obj._nBytes)
       << std::endl
       << obj._lengthDistribution << obj._l3Distribution << obj._l4Distribution
       << obj._uniqueFields << obj._tcpFlags
       << std::format("{}Packets with valid checksum: {}", ItemIndent,
                      obj._nValidChecksums)
       << std::endl
       << std::format("{}Packets with invalid checksum: {}", ItemIndent,
                      obj._nInvalidChecksums)
       << std::endl;

    return os;
}

} // namespace Statistics
