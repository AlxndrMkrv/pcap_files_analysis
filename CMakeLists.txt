cmake_minimum_required(VERSION 3.25)

project(pcap_files_analysis LANGUAGES CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# pcap library is required to build the project
find_library(PCAP_LIB pcap)
if(NOT PCAP_LIB)
    message(FATAL_ERROR "pcap library not found")
endif()

# Check endianness
if(NOT ${CMAKE_CXX_BYTE_ORDER} STREQUAL "LITTLE_ENDIAN")
    message(SEND_ERROR "${CMAKE_CXX_BYTE_ORDER} platforms are not supported")
endif()

# Disable this option to force the program fail on insane packets
option(IGNORE_INSANE "Don't stop when insane packet encountered" NO)

# Run on single thread
option(SINGLE_THREAD "Run a single thread in pool to debug parsing" NO)

# Use thread sanitizer
option(THREAD_SANITIZER "Compile with a thread sanitizer" NO)

# Some files to enable static checks
include(cmake/cppcheck.cmake)
include(cmake/clang-tidy.cmake)

# Miscellarious files just to see them in the project tree
add_custom_target(misc SOURCES .clang-format README.md)

#
# Build targets are divided to libraries just to get more convenient project
# tree in IDE
#

include_directories(src)

add_library(bytespan OBJECT src/BytesSpan.hxx
                            src/BytesSpan.cxx
)

add_library(logs OBJECT src/Log.hxx
                        src/Log.cxx
)

add_library(checksum OBJECT src/Checksum/RFC1071.hxx
                            src/Checksum/RFC1071.cxx
)

add_library(packets OBJECT src/Packet/Base.hxx
                           src/Packet/Ethernet.hxx
                           src/Packet/Ethernet.cxx
                           src/Packet/ICMP.hxx
                           src/Packet/ICMP.cxx
                           src/Packet/IPv4.hxx
                           src/Packet/IPv4.cxx
                           src/Packet/IPv6.hxx
                           src/Packet/IPv6.cxx
                           src/Packet/TCP.hxx
                           src/Packet/TCP.cxx
                           src/Packet/UDP.hxx
                           src/Packet/UDP.cxx
)
target_include_directories(packets PRIVATE Packet)

add_library(statistics OBJECT src/Statistics/_Common.hxx
                              src/Statistics/ChecksumValidity.hxx
                              src/Statistics/ChecksumValidity.cxx
                              src/Statistics/L3Distribution.hxx
                              src/Statistics/L3Distribution.cxx
                              src/Statistics/L4Distribution.hxx
                              src/Statistics/L4Distribution.cxx
                              src/Statistics/LengthDistribution.hxx
                              src/Statistics/LengthDistribution.cxx
                              src/Statistics/TcpFlags.hxx
                              src/Statistics/TcpFlags.cxx
                              src/Statistics/UniqueFields.hxx
                              src/Statistics/UniqueFields.cxx
)
target_include_directories(statistics PRIVATE Statistics)

add_executable(${PROJECT_NAME} src/main.cxx
                               src/Parser.hxx
                               src/Parser.cxx
                               src/Report.hxx
                               src/Report.cxx

)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_link_libraries(${PROJECT_NAME} PRIVATE
                      pcap checksum packets statistics bytespan logs)
target_compile_definitions(${PROJECT_NAME} PRIVATE
                           __PROGRAM_NAME="${PROJECT_NAME}"
)

# Apply options
if (${IGNORE_INSANE})
    target_compile_definitions(${PROJECT_NAME} PRIVATE __IGNORE_INSANE)
endif()

if (${SINGLE_THREAD})
    target_compile_definitions(${PROJECT_NAME} PRIVATE __SINGLE_THREAD)
endif()

if(${THREAD_SAFE})
    message(STATUS "Clang Thread Safety Analysis enabled")
    add_compile_options(-Wthread-safety)
endif()

if(${THREAD_SANITIZER})
    message(STATUS "Thread sanitizer enabled")
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
endif()

#
# Add tests
#

add_subdirectory(tests)

