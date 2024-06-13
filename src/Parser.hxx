#pragma once

#include "Report.hxx"
#include <condition_variable>
#include <queue>
#include <stop_token>
#include <thread>
#include <vector>

namespace fs = std::filesystem;

class Parser {
    using Guard = std::unique_lock<std::mutex>;

public:
    using Filenames = std::queue<std::string>;
    using Reports = std::deque<Report>;

    explicit Parser(
        const std::string_view directory,
        const size_t nThreads = std::thread::hardware_concurrency());
    ~Parser();

    /**
     * @brief process Adds filenames to parse. Blocks until all current files
     * are parsed
     * @param filenames
     */

    void process(Filenames && filenames);

    /**
     * @brief results Returns reports about parsed files. Blocks until all
     * files are parsed
     * @return
     */

    Reports && results();

private:
    void parse(Report & report);
    void threadLoop(const std::stop_token token);

    Filenames _files;
    Reports _results;

    std::vector<std::jthread> _threads;
    std::stop_source _stopSource;
    std::mutex _mutex;
    std::condition_variable _threadCondition;
    std::condition_variable _controlCondition;
    std::atomic_size_t _tasksCounter; // value-initialized since C++20

    const std::string _directory;
};
