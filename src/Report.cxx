#include "Report.hxx"
#include "Log.hxx"
#include "Packet/Ethernet.hxx"
#include "Packet/ICMP.hxx"
#include "Statistics/_Common.hxx"
#include <format>

constexpr const char * ItemIndent = Statistics::ItemIndent;

constexpr void HandleInsanity(const ::Packet::Abstract & pkt,
                              std::string_view filename)
{
#ifndef __IGNORE_INSANE
    pkt.print(std::cerr);
    Log::Fatal(std::format("^^^^ Insane packet found in \"{}\"", filename));
#endif
}

Report::Report(const std::filesystem::path & filename) : _filename(filename) {}

void Report::markInvalid() { _fileInvalid = true; }

const std::string & Report::filename() const { return _filename; }

void Report::analysePacket(const BytesSpan data)
{
    ++_nPackets;
    _nBytes += data.size();

    _lengthDistribution.update(data.size());

    const Ethernet::Packet pkt{data};

    if (!pkt.isSane())
        HandleInsanity(pkt, _filename);

    _uniqueFields.update(pkt);
    _l3Distribution.update(pkt.etherType());

    switch (pkt.etherType()) {
    case Ethernet::EtherType::IPV4:
        analyseIPv4(IPv4::Packet(pkt.payload()));
        break;
    case Ethernet::EtherType::IPV6:
        analyseIPv6(IPv6::Packet(pkt.payload()));
        break;
    default:
        break;
    }
}

void Report::analyseIPv4(const IPv4::Packet & pkt)
{
    if (!pkt.isSane())
        HandleInsanity(pkt, _filename);

    _l4Distribution.update(pkt.protocol());
    _l3Valids.update(pkt);
    _uniqueFields.update(pkt);

    switch (pkt.protocol()) {
    case IPv4::Protocol::TCP:
        analyseTCP(TCP::Packet(pkt.payload(), pkt.pseudoHeader()));
        break;
    case IPv4::Protocol::UDP:
        analyseUDP(UDP::Packet(pkt.payload(), pkt.pseudoHeader()));
        break;
    case IPv4::Protocol::ICMP:
        analyseICMP(ICMP::Packet(pkt.payload()));
        break;
    default:
        break;
    }
}

void Report::analyseIPv6(const IPv6::Packet & pkt)
{
    if (!pkt.isSane())
        HandleInsanity(pkt, _filename);

    _l4Distribution.update(pkt.nextHeader());
    _l3Valids.update(pkt);
    _uniqueFields.update(pkt);

    switch (pkt.nextHeader()) {
    case IPv4::Protocol::TCP:
        analyseTCP(TCP::Packet(pkt.payload(), pkt.pseudoHeader()));
        break;
    case IPv4::Protocol::UDP:
        analyseUDP(UDP::Packet(pkt.payload(), pkt.pseudoHeader()));
        break;
    case IPv4::Protocol::ICMP6:
        analyseICMP(ICMP::Packet{pkt.payload(), pkt.pseudoHeader()});
        break;
    default:
        break;
    }
}

void Report::analyseICMP(const ICMP::Packet & pkt)
{
    if (!pkt.isSane())
        HandleInsanity(pkt, _filename);

    _l4Valids.update(pkt);
}

void Report::analyseTCP(const TCP::Packet & pkt)
{
    if (!pkt.isSane())
        HandleInsanity(pkt, _filename);

    _uniqueFields.update(pkt);
    _tcpFlags.update(pkt.flags());
    _l4Valids.update(pkt);
}

void Report::analyseUDP(const UDP::Packet & pkt)
{
    if (!pkt.isSane())
        HandleInsanity(pkt, _filename);

    _uniqueFields.update(pkt);
    _l4Valids.update(pkt);
}

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
       << obj._lengthDistribution << std::endl
       << obj._l3Distribution << std::endl
       << obj._l4Distribution << std::endl
       << obj._uniqueFields << std::endl
       << obj._tcpFlags << std::endl
       << std::format("{}L3 packets checksum distribution:", ItemIndent)
       << std::endl
       << obj._l3Valids << std::endl
       << std::format("{}L4 packets checksum distribution:", ItemIndent)
       << std::endl
       << obj._l4Valids << std::endl;

    return os;
}
