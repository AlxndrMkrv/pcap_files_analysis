#include "Parser.hxx"
#include <filesystem>
#include <format>
#include <iostream>
#include <print>

#ifndef __SINGLE_THREAD
#include <algorithm>
#endif

#ifndef __PROGRAM_NAME
#error "Program must be compiled with __PROGRAM_NAME defined"
#endif

static constexpr char FileExtension[] = ".pcap";

namespace fs = std::filesystem;

constexpr void terminate(const std::string_view message, const int exitCode = 0)
{
    (exitCode == 0 ? std::cout : std::cerr) << message << std::endl;
    std::exit(exitCode);
}

int main(int argc, char * argv[])
{
    const std::span<const char * const> args(argv, argc);

    if (argc != 2)
        terminate(
            std::format("Usage: {} <path to *.pcap files>", __PROGRAM_NAME));

    const fs::path directory{args[1]};

    if (!fs::is_directory(directory))
        terminate(
            std::format("Error: \"{}\" is not a directory", directory.c_str()));

    Parser::Filenames filenames;

    for (const auto & entry : fs::directory_iterator(directory))
        if (entry.is_regular_file() &&
            entry.path().extension() == FileExtension)
            filenames.emplace(fs::relative(entry.path(), directory));

#ifndef __SINGLE_THREAD
    Parser parser(directory.string(),
                  std::min<size_t>(filenames.size(),
                                   std::thread::hardware_concurrency()));
#else
    Parser parser(directory.string(), 1);
#endif

    parser.process(std::move(filenames));

    const Parser::Reports reports{parser.results()};

    for (const auto & report : reports)
        std::cout << report << std::endl;
}
