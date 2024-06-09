cmake_minimum_required(VERSION 3.25)

project(pcap_files_analysis LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

find_library(PCAP_LIB pcap)
if(NOT PCAP_LIB)
    message(FATAL_ERROR "pcap library not found")
endif()

include(cmake/cppcheck.cmake)
include(cmake/clang-tidy.cmake)

add_custom_target(misc SOURCES .clang-format README.md)

add_executable(${PROJECT_NAME} src/main.cxx
                               src/Parser.hxx
                               src/Parser.cxx
                               src/Packet/Base.hxx
                               src/Packet/Ethernet.hxx
                               src/Packet/Ethernet.cxx
                               src/Packet/L2.hxx
                               src/Packet/L3.hxx
                               src/Packet/L4.hxx
                               src/Packet/Tcp.hxx
                               src/Statistics/_Common.hxx
                               src/Statistics/UniqueFields.hxx
                               src/Statistics/UniqueFields.cxx
                               src/Statistics/LengthDistribution.hxx
                               src/Statistics/LengthDistribution.cxx
                               src/Statistics/TcpFlags.hxx
                               src/Statistics/TcpFlags.cxx
                               src/Statistics/Report.hxx
                               src/Statistics/Report.cxx
)
target_compile_definitions(${PROJECT_NAME} PRIVATE
                           __PROGRAM_NAME="${PROJECT_NAME}")
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE pcap)
