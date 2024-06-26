#include "Parser.hxx"
#include <pcap.h>

/* ************************************************************************** */
/*                               Public methods                               */

Parser::Parser(const std::string_view directory, const size_t nThreads) :
    _directory{directory}
{
    for (size_t i = 0; i < nThreads; ++i)
        _threads.emplace_back(&Parser::threadLoop, this,
                              _stopSource.get_token());
}

Parser::~Parser()
{
    _stopSource.request_stop();
    _threadCondition.notify_all();
}

void Parser::process(Parser::Filenames && filenames)
{
    {
        Guard lock{_mutex};

        _controlCondition.wait(
            lock, [this]() { return _files.empty() && _tasksCounter == 0; });

        _tasksCounter = filenames.size();
        _files = std::move(filenames);
    }

    _threadCondition.notify_all();
}

Parser::Reports && Parser::results()
{
    Guard lock{_mutex};

    _controlCondition.wait(
        lock, [this]() { return _files.empty() && _tasksCounter == 0; });

    return std::move(_results);
}

/* ************************************************************************** */
/*                              Private methods                               */

void Parser::threadLoop(const std::stop_token token)
{
    Report * report = nullptr;

    while (!token.stop_requested()) {
        {
            Guard lock{_mutex};

            _threadCondition.wait(lock, [this, &token]() {
                return token.stop_requested() || !_files.empty();
            });

            if (token.stop_requested()) {
                _threadCondition.notify_one();
                break;
            }

            _results.emplace_back(std::move(_files.front()));
            report = &_results.back();
            _files.pop();
        }

        parse(*report);

        if (--_tasksCounter == 0)
            _controlCondition.notify_one();
    }
}

struct PcapDeleter {
    void operator()(pcap_t * handle) const
    {
        if (handle != nullptr) {
            pcap_close(handle);
        }
    }
};

using PcapHandle = std::unique_ptr<pcap_t, PcapDeleter>;

void Parser::parse(Report & report) const
{
    std::array<char, PCAP_ERRBUF_SIZE> errbuf;

    const PcapHandle handle{pcap_open_offline(
        std::format("{}/{}", _directory, report.filename()).c_str(),
        static_cast<char *>(errbuf.begin()))};

    if (handle == nullptr) {
        report.markInvalid();
        return;
    }

    pcap_pkthdr * header = nullptr;
    const uint8_t * data = nullptr;
    int retValue = 0;
    size_t idx = 0;

    while ((retValue = pcap_next_ex(handle.get(), &header, &data)) >= 0) {
        ++idx;
        if (header->caplen != header->len) {
            std::cerr << std::format("{}: skipping packet No. {}. {} bytes of "
                                     "{} are not captured",
                                     report.filename(), idx,
                                     header->len - header->caplen, header->len)
                      << std::endl;
            continue;
        }
        report.analysePacket({data, header->caplen});
    }

    if (retValue != PCAP_ERROR_BREAK)
        report.markInvalid();
}
