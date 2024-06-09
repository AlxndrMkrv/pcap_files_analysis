#include "Parser.hxx"
#include <algorithm>
#include <filesystem>
#include <format>
#include <iostream>
#include <print>

#ifndef __PROGRAM_NAME
#error "Program must be compiled with __PROGRAM_NAME defined"
#endif

static constexpr char FileExtension[] = ".pcap";

/* packets:
 * Ethernet, Unknown
 * IPv4, IPv6, other
 * TCP, UDP, ICMP, ICMPv6, other
 * */

namespace fs = std::filesystem;

constexpr void terminate(const std::string_view message, const int exitCode = 0)
{
    exitCode ? std::cerr : std::cout << message << std::endl;
    std::exit(exitCode);
}

int main(int argc, char * argv[])
{
    std::span<const char * const> args(argv, argc);

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

    Parser parser(directory.string(),
                  std::min<size_t>(filenames.size(),
                                   std::thread::hardware_concurrency()));

    parser.process(std::move(filenames));

    const Parser::Reports reports{parser.results()};

    for (const auto & report : reports)
        std::cout << report << std::endl;
}