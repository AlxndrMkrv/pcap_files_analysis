#pragma once

#include "Packet/ICMP.hxx"
#include "Statistics/ChecksumValidity.hxx"
#include "Statistics/L3Distribution.hxx"
#include "Statistics/L4Distribution.hxx"
#include "Statistics/LengthDistribution.hxx"
#include "Statistics/TcpFlags.hxx"
#include "Statistics/UniqueFields.hxx"
#include <filesystem>

class Report {
public:
    Report(const std::filesystem::path & filename);

    void markInvalid();
    const std::string & filename() const;
    void analysePacket(const BytesSpan data);

private:
    void analyseIPv4(const IPv4::Packet & pkt);
    void analyseIPv6(const IPv6::Packet & pkt);
    void analyseICMP(const ICMP::Packet & pkt);
    void analyseTCP(const TCP::Packet & pkt);
    void analyseUDP(const UDP::Packet & pkt);

    friend std::ostream & operator<<(std::ostream & os, const Report & obj);

private:
    std::string _filename;
    bool _fileInvalid = false;
    size_t _nPackets = 0;
    size_t _nBytes = 0;
    Statistics::LengthDistribution _lengthDistribution;
    Statistics::L3Distribution _l3Distribution;
    Statistics::L4Distribution _l4Distribution;
    Statistics::TcpFlags _tcpFlags;
    Statistics::UniqueFields _uniqueFields;
    Statistics::ChecksumValidity _l3Valids;
    Statistics::ChecksumValidity _l4Valids;
};
