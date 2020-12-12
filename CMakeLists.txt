cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----

# Note: update this to your new project's name and version
project(
        atl_expected
        VERSION 1.0
        LANGUAGES CXX
)

# ---- Fetch ----

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.13.1)

FetchContent_MakeAvailable(Catch2)

# ---- Include guards ----

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif ()

# ---- Add source files ----

file(GLOB_RECURSE headers CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/include/atl/expected.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/atl/bad_expected_access.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/atl/expected_t.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/atl/success.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/atl/unexpected_t.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/atl/detail/expected_five_bases.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/atl/detail/expected_operations_base.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/atl/detail/expected_storage_base.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/atl/detail/functional_extensions.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/atl/detail/no_init_t.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/atl/detail/traits.h"
        )

# ---- Create library ----

add_library(atl_expected INTERFACE)
set_target_properties(atl_expected PROPERTIES INTERFACE_COMPILE_FEATURES cxx_std_17)

# being a cross-platform target, we enforce standards conformance on MSVC
target_compile_options(atl_expected INTERFACE "$<$<BOOL:${MSVC}>:/permissive->")

# Link dependencies (if required) target_link_libraries(atl_expected PUBLIC cxxopts)
target_include_directories(
        atl_expected INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)

# ---- Create tests ----

set(test_sources
        tests/assignment.cpp
        tests/bases.cpp
        tests/constexpr.cpp
        tests/constructors.cpp
        tests/emplace.cpp
        tests/extensions.cpp
        tests/issues.cpp
        tests/main.cpp
        tests/noexcept.cpp
        tests/observers.cpp
        tests/relops.cpp
        tests/success.cpp
        tests/swap.cpp
        )
add_executable(atl_expected_tests ${test_sources})
target_link_libraries(atl_expected_tests PUBLIC Catch2 atl_expected)
add_test(NAME atl_expected COMMAND atl_expected_tests)