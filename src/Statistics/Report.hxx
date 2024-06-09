#pragma once

#include "LengthDistribution.hxx"
#include "UniqueFields.hxx"
#include <filesystem>

namespace Statistics {

class Report {
public:
    Report(const std::filesystem::path & filename);

    void markInvalid();
    const std::string & filename() const;
    void analysePacket(std::span<const uint8_t> data);

    friend std::ostream & operator<<(std::ostream & os, const Report & obj);

private:
    std::string _filename;
    bool _fileInvalid = false;
    size_t _nPackets = 0;
    size_t _nBytes = 0;
    LengthDistribution _lengthDistribution;
    // L3Distribution _
    // L4Distribution _
    // TcpFlags _tcpFlags
    UniqueFields _uniqueFields;
    size_t _nValidChecksums = 0;
    size_t _nInvalidChecksums = 0;
};

} // namespace Statistics
