# scnlib

[![Ubuntu 20 builds](https://github.com/eliaskosunen/scnlib/actions/workflows/ubuntu-20.yml/badge.svg?branch=master)](https://github.com/eliaskosunen/scnlib/actions/workflows/ubuntu-20.yml)
[![Ubuntu 18 builds](https://github.com/eliaskosunen/scnlib/actions/workflows/ubuntu-18.yml/badge.svg?branch=master)](https://github.com/eliaskosunen/scnlib/actions/workflows/ubuntu-18.yml)
[![macOS builds](https://github.com/eliaskosunen/scnlib/actions/workflows/macos.yml/badge.svg?branch=master)](https://github.com/eliaskosunen/scnlib/actions/workflows/macos.yml)
[![Windows builds](https://github.com/eliaskosunen/scnlib/actions/workflows/windows.yml/badge.svg?branch=master)](https://github.com/eliaskosunen/scnlib/actions/workflows/windows.yml)
[![Alpine builds](https://github.com/eliaskosunen/scnlib/actions/workflows/alpine.yml/badge.svg?branch=master)](https://github.com/eliaskosunen/scnlib/actions/workflows/alpine.yml)
[![Code Coverage](https://codecov.io/gh/eliaskosunen/scnlib/branch/master/graph/badge.svg?token=LyWrDluna1)](https://codecov.io/gh/eliaskosunen/scnlib)

[![Latest Release](https://img.shields.io/github/v/release/eliaskosunen/scnlib?sort=semver)](https://github.com/eliaskosunen/scnlib/releases)
[![License](https://img.shields.io/github/license/eliaskosunen/scnlib.svg)](https://github.com/eliaskosunen/scnlib/blob/master/LICENSE)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-11%2F14%2F17%2F20-blue.svg)](https://img.shields.io/badge/C%2B%2B-11%2F14%2F17%2F20-blue.svg)

```cpp
#include <scn/scn.h>
#include <cstdio>

int main() {
    int i;
    // Read an integer from stdin
    // with an accompanying message
    scn::prompt("What's your favorite number? ", "{}", i);
    printf("Oh, cool, %d!", i);
}

// Example result:
// What's your favorite number? 42
// Oh, cool, 42!
```

## What is this?

`scnlib` is a modern C++ library for replacing `scanf` and `std::istream`.
This library attempts to move us ever so closer to replacing `iostream`s and C stdio altogether.
It's faster than `iostream` (see Benchmarks) and type-safe, unlike `scanf`.
Think [{fmt}](https://github.com/fmtlib/fmt) but in the other direction.

This library is the reference implementation of the ISO C++ standards proposal
[P1729 "Text Parsing"](https://wg21.link/p1729).

This library is currently of pre-release quality (version 1.0-rc1),
but it's close to being stable.
1.0 will be released when an rc-version will prove itself reasonably bug-free.

## Documentation

The documentation can be found online, from https://scnlib.readthedocs.io.

To build the docs yourself, build the `doc` and `doc-sphinx` targets generated by CMake.
The `doc` target requires Doxygen, and `doc-sphinx` requires Python 3.8, Sphinx and Breathe.

## Examples

### Reading a `std::string`

```cpp
#include <scn/scn.h>
#include <iostream>
#include <string_view>

int main() {
    std::string word;
    auto result = scn::scan("Hello world", "{}", word);

    std::cout << word << '\n'; // Will output "Hello"
    std::cout << result.range_as_string() << '\n';  // Will output " world!"
}
```

### Reading multiple values

```cpp
#include <scn/scn.h>

int main() {
    int i, j;
    auto result = scn::scan("123 456 foo", "{} {}", i, j);
    // result == true
    // i == 123
    // j == 456

    std::string str;
    ret = scn::scan(result.range(), "{}", str);
    // result == true
    // str == "foo"
}
```

### Using the `tuple`-return API

```cpp
#include <scn/scn.h>
#include <scn/tuple_return.h>

int main() {
    auto [r, i] = scn::scan_tuple<int>("42", "{}");
    // r is a result object, contextually convertible to `bool`
    // i == 42
}
```

### Error handling

```cpp
#include <scn/scn.h>
#include <string_view>
#include <iostream>

int main() {
    int i;
    // "foo" is not a valid integer
    auto result = scn::scan("foo", "{}", i);
    if (!result) {
        // i is not touched (still unconstructed)
        // result.range() == "foo" (range not advanced)
        std::cout << "Integer parsing failed with message: " << result.error().msg() << '\n';
    }
}
```

## Features

 - Blazing-fast parsing of values (see benchmarks)
 - Modern C++ interface, featuring type safety (variadic templates), convenience (ranges) and customizability
   - No << chevron >> hell
   - Requires C++11 or newer
 - "{python}"-like format string syntax
 - Optionally header only
 - Minimal code size increase (see benchmarks)
 - No exceptions (supports building with `-fno-exceptions -fno-rtti` with minimal loss of functionality)
   - Localization requires exceptions, because of the way `std::locale` is

## Installing

`scnlib` uses CMake.
If your project already uses CMake, integration is easy.
First, clone, build, and install the library

```sh
# Whereever you cloned scnlib to
$ mkdir build
$ cd build
$ cmake ..
$ make -j
$ make install
```

Then, in your project:

```cmake
# Find scnlib package
find_package(scn CONFIG REQUIRED)

# Target which you'd like to use scnlib
# scn::scn-header-only to use the header-only version
add_executable(my_program ...)
target_link_libraries(my_program scn::scn)
```

Alternatively, if you have `scnlib` downloaded somewhere, or maybe even bundled inside your project (like a git submodule),
you can use `add_subdirectory`:

```cmake
add_subdirectory(path/to/scnlib)

# like above
add_executable(my_program ...)
target_link_libraries(my_program scn::scn)
```

See docs for usage without CMake.

## Compiler support

Every commit is tested with
 * gcc 5.5 and newer (until v11)
 * clang 6.0 and newer (until v13)
 * Visual Studio 2019 and 2022
 * clang 12 and gcc 11 on macOS Catalina

with very extreme warning flags (see cmake/flags.cmake) and with multiple build configurations for each compiler.

Other compilers and compiler versions may work, but it is not guaranteed.
If your compiler does not work, it may be a bug in the library.
However, support will not be provided for:

 * GCC 4.9 (or earlier): C++11 support is too buggy
 * VS 2015 (or earlier): unable to handle templates

VS 2017 is not tested, as GitHub Actions has deprecated the support for it.
The last commit tested and verified to work with VS 2017 is
[32be3f9](https://github.com/eliaskosunen/scnlib/commit/32be3f9) (post-v0.4).

The code is only tested on amd64 machines (both win32 and win64 on Windows),
because that's the only architecture GitHub Actions has runners for.
The last commit tested and verified to work with arm64 is
[7b93942](https://github.com/eliaskosunen/scnlib/commit/7b93942) (v0.4).

## Benchmarks

### Run-time performance

![Benchmark results](benchmark/runtime/results.png?raw=true "Benchmark results")

These benchmarks were run on a Ubuntu 20.04 machine running kernel version 5.4.0-52, with an Intel Core i5-6600K processor, and compiled with gcc version 9.3.0, with `-O3 -DNDEBUG -march=native`.
The source code for the benchmarks can be seen in the `benchmark` directory.

You can run the benchmarks yourself by enabling `SCN_BENCHMARKS`.
`SCN_BENCHMARKS` is enabled by default if `scn` is the root CMake project, and disabled otherwise.

```sh
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release -DSCN_BENCHMARKS=ON -DSCN_USE_NATIVE_ARCH=ON -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON ..
$ make -j
# choose benchmark to run in ./benchmark/runtime/*/bench-*
$ ./benchmark/runtime/integer/bench-int
```

Performance comparison benchmarks with Boost.Spirit.x3 can be found [here](https://github.com/eliaskosunen/scnlib-spirit-benchmark)

Times are in nanoseconds of CPU time. Lower is better.

#### Integer parsing (`int`)

| Test   | `std::stringstream` | `sscanf` | `scn::scan` | `scn::scan_default` |
| :----- | ------------------: | -------: | ----------: | ------------------: |
| Test 1 | 274                 | 96.5     | 43.0        | 40.3                |
| Test 2 | 77.7                | 526      | 68.1        | 60.5                |

#### Floating-point parsing (`double`)

| Test   | `std::stringstream` | `sscanf` | `scn::scan` |
| :----- | ------------------: | -------: | ----------: |
| Test 1 | 416                 | 164      | 167         |
| Test 2 | 223                 | 570      | 195         |

#### Reading random whitespace-separated strings

| Character type | `scn::scan` | `scn::scan` and `string_view` | `std::stringstream` |
| :------------- | ----------: | ----------------------------: | ------------------: |
| `char`         | 40.7        | 38.0                          | 50.2                |
| `wchar_t`      | 42.7        | 38.3                          | 122                 |

#### Test 1 vs. Test 2

In the above comparisons:

 * "Test 1" refers to parsing a single value from a string which only contains the string representation for that value.
   The time used for constructing parser state is included.
   For example, the source string could be `"123"`.
   In this case, a parser is constructed, and a value (`123`) is parsed.
   This test is called "single" in the benchmark sources.
 * "Test 2" refers to the average time of parsing a value from a string containing multiple string representations separated by spaces.
   The time used for constructing parser state is not included.
   For example, the source string could be `"123 456"`.
   In this case, a parser is constructed before the timer is started.
   Then, a single value is read from the source, and the source is advanced to the start of the next value.
   The time it took to parse a single value is averaged out.
   This test is called "repeated" in the benchmark sources.

### Executable size

Executable size benchmarks test generated code bloat for nontrivial projects.
It generates 25 translation units and reads values from stdin five times to simulate a medium sized project.
The resulting executable size is shown in the following tables and graphs.
The "stripped size" metric shows the size of the executable after running `strip`.

The code was compiled on Ubuntu 20.04 with g++ 9.3.0.
`scnlib` is linked dynamically to level out the playing field compared to already dynamically linked `libc` and `libstdc++`.
See the directory `benchmark/bloat` for more information, e.g. templates for each TU.

To run these tests yourself:

```sh
$ cd build
# For Debug
$ cmake -DCMAKE_BUILD_TYPE=Debug -DSCN_BUILD_BLOAT=ON -DSCN_BUILD_BUILDTIME=OFF -DSCN_TESTS=OFF -DBUILD_SHARED_LIBS=ON -DSCN_INSTALL=OFF ..
# For Release
$ cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON -DSCN_BUILD_BLOAT=ON -DSCN_BUILD_BUILDTIME=OFF -DSCN_TESTS=OFF DBUILD_SHARED_LIBS=ON -DSCN_INSTALL=OFF ..
# For Minimized Release
$ cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON -DSCN_BUILD_BLOAT=ON -DSCN_BUILD_BUILDTIME=OFF -DSCN_TESTS=OFF DBUILD_SHARED_LIBS=ON -DSCN_INSTALL=OFF ..

$ make -j
$ ./benchmark/bloat/run-bloat-tests.py ./benchmark/bloat
```

Sizes are in kibibytes (KiB).
Lower is better.

#### Minimized build (-Os -DNDEBUG)

| Method                          | Executable size | Stripped size |
| :------------------------------ | --------------: | ------------: |
| empty                           |            16.0 |          14.0 |
| `std::scanf`                    |            17.8 |          14.2 |
| `std::istream`                  |            19.4 |          14.2 |
| `scn::input`                    |            19.1 |          14.2 |
| `scn::input` (header-only)      |            41.4 |          30.2 |
| `scn::scan_value`               |            35.9 |          26.2 |
| `scn::scan_value` (header-only) |            32.0 |          22.2 |

![Benchmark results](benchmark/bloat/results_minsizerel.png?raw=true "Benchmark results")

#### Release build (-O3 -DNDEBUG)

| Method                          | Executable size | Stripped size |
| :------------------------------ | --------------: | ------------: |
| empty                           |            16.0 |          14.0 |
| `std::scanf`                    |            17.7 |          14.2 |
| `std::istream`                  |            19.4 |          14.2 |
| `scn::input`                    |            19.1 |          14.2 |
| `scn::input` (header-only)      |            53.8 |          42.2 |
| `scn::scan_value`               |            39.3 |          30.2 |
| `scn::scan_value` (header-only) |            43.8 |          34.2 |

![Benchmark results](benchmark/bloat/results_release.png?raw=true "Benchmark results")

#### Debug build (-g)

| Method                          | Executable size | Stripped size |
| :------------------------------ | --------------: | ------------: |
| empty                           |            35.9 |          14.0 |
| `std::scanf`                    |             614 |          18.2 |
| `std::istream`                  |             688 |          26.2 |
| `scn::input`                    |            1425 |          42.3 |
| `scn::input` (header-only)      |            4703 |           202 |
| `scn::scan_value`               |            3622 |           134 |
| `scn::scan_value` (header-only) |            4778 |           186 |

![Benchmark results](benchmark/bloat/results_debug.png?raw=true "Benchmark results")

### Build time

This test measures the time it takes to compile a binary when using different libraries.
Note, that the time it takes to compile the library is not taken into account (unfair measurement against precompiled stdlibs).

These tests were run on an Ubuntu 20.04 machine with an i5-6600K and 16 GB of RAM, using GCC 9.3.0.
The compiler flags for a debug build were `-g`, and `-O3 -DNDEBUG` for a release build.

To run these tests yourself, enable CMake flag `SCN_BUILD_BUILDTIME`.
In order for these tests to work, `c++` must point to a gcc-compatible C++ compiler binary,
and a POSIX-compatible `/usr/bin/time` must be present.

```sh
$ cd build
$ cmake -DSCN_BUILD_BUILDTIME=ON ..
$ make -j
$ ./benchmark/buildtime/run-buildtime-tests.sh
```

#### Build time

Time is in seconds of CPU time (user time + sys/kernel time).
Lower is better.

| Method                      | Debug | Release |
| :-------------------------- | ----: | ------: |
| empty                       | 0.04  | 0.04    |
| `scanf`                     | 0.25  | 0.23    |
| `std::istream` / `std::cin` | 0.31  | 0.29    |
| `scn::input`                | 0.50  | 0.48    |
| `scn::input` (header only)  | 1.34  | 2.48    |

#### Memory consumption

Memory is in mebibytes (MiB).
Lower is better.

| Method                      | Debug | Release |
| :-------------------------- | ----: | ------: |
| empty                       | 22.2  | 23.8    |
| `scanf`                     | 46.9  | 46.7    |
| `std::istream` / `std::cin` | 55.1  | 54.8    |
| `scn::input`                | 78.0  | 75.6    |
| `scn::input` (header only)  | 140   | 166     |

## Acknowledgements

The contents of this library are heavily influenced by {fmt} and its derivative works.  
<https://github.com/fmtlib/fmt>  

The bundled ranges implementation found from this library is based on NanoRange:  
<https://github.com/tcbrindle/NanoRange>

The default floating-point parsing algorithm used by this library is implemented by fast_float:  
<https://github.com/fastfloat/fast_float>

The Unicode-related parts of this library are based on utfcpp:  
<https://github.com/nemtrif/utfcpp>

The design of this library is also inspired  by the Python `parse` library:  
<https://github.com/r1chardj0n3s/parse>

## License

scnlib is licensed under the Apache License, version 2.0.  
Copyright (c) 2017 Elias Kosunen  
See LICENSE for further details

See the directory `licenses/` for third-party licensing information.
