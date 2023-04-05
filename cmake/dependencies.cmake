include(FetchContent)

set(SCN_OPTIONAL_DEPENDENCIES "")

if (SCN_TESTS)
    # GTest

    FetchContent_Declare(
            googletest
            GIT_REPOSITORY  https://github.com/google/googletest.git
            GIT_TAG         main
    )

    # gtest CMake does some flag overriding we don't want, and it's also quite heavy
    # Do it manually

    FetchContent_GetProperties(googletest)
    if(NOT googletest)
        FetchContent_Populate(googletest)
    endif()

    find_package(Threads)

    add_library(scn_gtest
            "${googletest_SOURCE_DIR}/googletest/src/gtest-all.cc"
            "${googletest_SOURCE_DIR}/googlemock/src/gmock-all.cc"
            )
    target_include_directories(scn_gtest SYSTEM
            PUBLIC
            "${googletest_SOURCE_DIR}/googletest/include"
            "${googletest_SOURCE_DIR}/googlemock/include"
            PRIVATE
            "${googletest_SOURCE_DIR}/googletest"
            "${googletest_SOURCE_DIR}/googlemock"
            )
    target_link_libraries(scn_gtest PRIVATE Threads::Threads)
    target_compile_features(scn_gtest PUBLIC cxx_std_14)
endif()

if (SCN_BENCHMARKS)
    # Google Benchmark

    set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "Turn off google benchmark tests")
    FetchContent_Declare(
            google-benchmark
            GIT_REPOSITORY  https://github.com/google/benchmark.git
            GIT_TAG         main
    )
    list(APPEND SCN_OPTIONAL_DEPENDENCIES "google-benchmark")
endif()

# range-v3

#FetchContent_Declare(
#        range-v3
#        GIT_REPOSITORY  https://github.com/ericniebler/range-v3
#        GIT_TAG         0.12.0
#        GIT_SHALLOW     TRUE
#)

# simdutf

FetchContent_Declare(
        simdutf
        GIT_REPOSITORY  https://github.com/simdutf/simdutf.git
        GIT_TAG         v3.2.4
        GIT_SHALLOW     TRUE
)

# fast_float

FetchContent_Declare(
        fast_float
        GIT_REPOSITORY  https://github.com/fastfloat/fast_float.git
        GIT_TAG         v4.0.0
        GIT_SHALLOW     TRUE
)

set(SIMDUTF_BENCHMARKS OFF)
set(BUILD_TESTING_BEFORE_SIMDUTF ${BUILD_TESTING})
set(BUILD_TESTING OFF)

FetchContent_GetProperties(simdutf)
if(NOT simdutf_POPULATED)
    FetchContent_Populate(simdutf)

    add_subdirectory(${simdutf_SOURCE_DIR} ${simdutf_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set(BUILD_TESTING ${BUILD_TESTING_BEFORE_SIMDUTF})

# make available

FetchContent_MakeAvailable(
        #range-v3
        fast_float
        ${SCN_OPTIONAL_DEPENDENCIES}
)
