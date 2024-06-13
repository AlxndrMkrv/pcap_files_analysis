
add_custom_target(tests)

function(add_test_ NAME SOURCE)
    set(multiValueArgs DEPS)
    cmake_parse_arguments(P "" "" "${multiValueArgs}" ${ARGN})

    get_filename_component(TARGET ${SOURCE} NAME_WE)

    add_executable(${TARGET} EXCLUDE_FROM_ALL ${SOURCE})
    target_link_libraries(${TARGET} PRIVATE ${P_DEPS} bytespan)
    add_test(NAME ${NAME} COMMAND ${TARGET})

    add_dependencies(tests ${TARGET})
endfunction()


add_test_("RFC1071 checksum" testRFC1071.cxx DEPS checksums)
add_test_("IPv4" testIPv4.cxx DEPS packets checksums)
add_test_("IPv6" testIPv6.cxx DEPS packets checksums)
add_test_("TCP" testTCP.cxx DEPS packets checksums)
add_test_("ICMP" testICMP.cxx DEPS packets checksums)

