
# Add -D CPPCHECK=[YES, <path_to_cppcheck>] to enable the static code
# analysis during the build step.

set(CPPCHECK NO CACHE STRING "Enable cppcheck static analyzer")

# don't go further if cppcheck is off
if(CPPCHECK STREQUAL NO)
	message(STATUS "Cppcheck static analyzer is disabled")
	return()
endif()

# find the cppcheck in the PATH
if(CPPCHECK STREQUAL YES)
	find_program(CPPCHECK_EXEC NAMES "cppcheck" REQUIRED)
# check provided path to a cppcheck
elseif(EXISTS ${CPPCHECK})
	set(CPPCHECK_EXEC ${CPPCHECK})
	cmake_path(ABSOLUTE_PATH CPPCHECK_EXEC BASE_DIRECTORY ${CMAKE_BINARY_DIR}
	           NORMALIZE)
# show error on an invalid argument
else()
	message(FATAL_ERROR "CPPCHECK variable must be one of "
	                    "[NO (default), YES, <path>], \"${CPPCHECK}\" given")
endif()

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/cppcheck_build_dir)

set(CXX_CPPCHECK "--inconclusive"
                 "--std=c++23"
                 "--force"
                 "--quiet"
                 "--inline-suppr"
                 "--enable=all"
                 "--library=gnu"
                 "--library=posix"
                 "--check-level=exhaustive"
                 "--checkers-report=${CMAKE_BINARY_DIR}/cppcheck_report"
                 "--cppcheck-build-dir=${CMAKE_BINARY_DIR}/cppcheck_build_dir"
                 "-U DOXYGEN"
)

set(CMAKE_CXX_CPPCHECK ${CPPCHECK_EXEC} ${CXX_CPPCHECK})

message(STATUS "Cppcheck [${CPPCHECK_EXEC}] analyzer will be enabled "
               "during the build step.")
