
# Add -D CLANG_TIDY=[YES, <path_to_clang-tidy>] to enable static code analysis
# with clang-tidy during the build step

set(CLANG_TIDY NO CACHE STRING "Enable clang-tidy static analyzer")

# don't go further if clang-tidy is off
if(CLANG_TIDY STREQUAL NO)
	message(STATUS "Clang-tidy static analyzer is disabled")
	return()
endif()

# find the clang-tidy in the PATH
if(CLANG_TIDY STREQUAL YES)
	find_program(CLANG_TIDY_EXEC NAMES "clang-tidy" REQUIRED)
# check provided path to a clang-tidy
elseif(EXISTS ${CLANG_TIDY})
	set(CLANG_TIDY_EXEC ${CLANG_TIDY})
	cmake_path(ABSOLUTE_PATH CLANG_TIDY_EXEC BASE_DIRECTORY ${CMAKE_BINARY_DIR}
	           NORMALIZE)
# show error on an invalid argument
else()
	message(FATAL_ERROR "CLANG_TIDY variable must be one of "
	                    "[NO (default), YES, <path>], \"${CLANG_TIDY}\" given")
endif()

# prepare a list for the -checks=<...> option
set(CLANG_TIDY_ARGS
    -*
    android-*
    bugprone-*
    cert-*
    cppcoreguidelines-*
    google-build-namespaces
    google-readability-casting
    google-runtime-memset
    llvm-namespace-comment
    llvm-twine-local
    misc-*
    modernize-use-nullptr
    modernize-redundant-void-arg
    modernize-use-override
    modernize-redundant-void-arg
    modernize-use-bool-literals
    perfomance-*
    readability-*
    -bugprone-reserved-identifier
    -cppcoreguidelines-avoid-c-arrays
    -cppcoreguidelines-avoid-const-or-ref-data-members
    -cppcoreguidelines-avoid-magic-numbers
    -cppcoreguidelines-pro-type-member-init
    -cppcoreguidelines-pro-type-union-access
    -cppcoreguidelines-non-private-member-variables-in-classes
    -cppcoreguidelines-pro-type-reinterpret-cast
    -cppcoreguidelines-special-member-functions
    -readability-avoid-const-params-in-decls
    -readability-avoid-nested-conditional-operator
    -readability-braces-around-statements
    -readability-identifier-length
    -readability-magic-numbers
    -readability-simplify-boolean-expr
    -misc-include-cleaner
    -misc-non-private-member-variables-in-classes
    -cert-dcl37-c
    -cert-dcl51-cpp
)

# convert the list to the comma-separated string
string(REPLACE ";" "," CLANG_TIDY_ARGS "${CLANG_TIDY_ARGS}")

# initialize cmake variable
set(CMAKE_CXX_CLANG_TIDY
    ${CLANG_TIDY_EXEC}
    -warnings-as-errors=*
    -header-filter=.*
    -checks=${CLANG_TIDY_ARGS}
)

message(STATUS "Clang-tidy [${CLANG_TIDY_EXEC}] analyzer will be enabled "
               "during the build step")
