#pragma once

#include "L3Distribution.hxx"
#include "L4Distribution.hxx"
#include "LengthDistribution.hxx"
#include "TcpFlags.hxx"
#include "UniqueFields.hxx"
#include <filesystem>

namespace Statistics {

class Report {
public:
    Report(const std::filesystem::path & filename);

    void markInvalid();
    const std::string & filename() const;
    void analysePacket(const std::span<const uint8_t> data);

private:
    void analyseIPv4(const std::span<const uint8_t> data);
    void analyseIPv6(const std::span<const uint8_t> data);
    void analyseICMP(const std::span<const uint8_t> data);
    void analyseTCP(const std::span<const uint8_t> data);
    void analyseUDP(const std::span<const uint8_t> data);

    friend std::ostream & operator<<(std::ostream & os, const Report & obj);

private:
    std::string _filename;
    bool _fileInvalid = false;
    size_t _nPackets = 0;
    size_t _nBytes = 0;
    LengthDistribution _lengthDistribution;
    L3Distribution _l3Distribution;
    L4Distribution _l4Distribution;
    TcpFlags _tcpFlags;
    UniqueFields _uniqueFields;
    size_t _nValidChecksums = 0;
    size_t _nInvalidChecksums = 0;
};

} // namespace Statistics
