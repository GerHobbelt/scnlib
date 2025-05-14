// Copyright 2017 Elias Kosunen
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file is a part of scnlib:
//     https://github.com/eliaskosunen/scnlib

#include <scn/scan.h>

#include "monolithic_examples.h"



#if defined(BUILD_MONOLITHIC)
#define main(cnt, arr)      scanf_example_02_main(cnt, arr)
#endif

int main(int argc, const char** argv)
{
    if (const auto [result, name] =
            scn::prompt<std::string>(
                "Input your full name:\n", "{:[^\n]}");
        result) {
        std::printf("Hello, %s", name.c_str());
    }
    else {
        std::puts("Couldn't read name");
    }
	return 0;
}
