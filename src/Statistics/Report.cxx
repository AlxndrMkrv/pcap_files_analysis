#include "Report.hxx"
#include "Packet/Ethernet.hxx"
#include "_Common.hxx"
#include <format>

namespace Statistics {

Report::Report(const std::filesystem::path & filename) : _filename(filename) {}

void Report::markInvalid() { _fileInvalid = true; }

const std::string & Report::filename() const { return _filename; }

void Report::analysePacket(std::span<const uint8_t> data)
{
    ++_nPackets;
    _nBytes += data.size();

    _lengthDistribution.update(data.size());

    if (data.size() <= Packet::Ethernet::MinimalSize)
        return;
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
       << obj._lengthDistribution
       //<< std::format("{}L3 packets distribution:", ItemIndent)
       /*<< obj._l3*/
       //<< std::format("{}L4 packets distribution:", ItemIndent)
       /* << obj._l4*/
       //<< std::endl
       << obj._uniqueFields
       << std::endl
       //<< std::format("{}TCP packets distribution by flags:", ItemIndent)
       /* << obj._tcpFlags*/
       << std::format("{}Packets with valid checksum: {}", ItemIndent,
                      obj._nValidChecksums)
       << std::endl
       << std::format("{}Packets with invalid checksum: {}", ItemIndent,
                      obj._nInvalidChecksums)
       << std::endl;

    return os;
}

} // namespace Statistics
