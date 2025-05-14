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

#include "wrapped_gtest.h"

#include <scn/scan.h>

using namespace std::string_view_literals;

TEST(UnicodeTest, Utf8CpLength)
{
    EXPECT_EQ(scn::detail::utf8_code_point_length_by_starting_code_unit('a'),
              1);
    EXPECT_EQ(scn::detail::utf8_code_point_length_by_starting_code_unit(
                  uint8_t{0xc4}),
              2);
    EXPECT_EQ(scn::detail::utf8_code_point_length_by_starting_code_unit(
                  uint8_t{0xe4}),
              3);
    EXPECT_EQ(scn::detail::utf8_code_point_length_by_starting_code_unit(
                  uint8_t{0xf4}),
              4);
    EXPECT_EQ(scn::detail::utf8_code_point_length_by_starting_code_unit(
                  uint8_t{0xa4}),
              0);
}

TEST(UnicodeTest, Utf8Decode)
{
    EXPECT_EQ(scn::detail::decode_utf8_code_point_exhaustive("a"),
              static_cast<char32_t>('a'));
    EXPECT_EQ(scn::detail::decode_utf8_code_point_exhaustive_valid("a"),
              static_cast<char32_t>('a'));

    EXPECT_EQ(scn::detail::decode_utf8_code_point_exhaustive("ä"),
              char32_t{0xe4});
    EXPECT_EQ(scn::detail::decode_utf8_code_point_exhaustive_valid("ä"),
              char32_t{0xe4});

    EXPECT_EQ(scn::detail::decode_utf8_code_point_exhaustive("�"),
              char32_t{0xfffd});
    EXPECT_EQ(scn::detail::decode_utf8_code_point_exhaustive_valid("�"),
              char32_t{0xfffd});

    EXPECT_EQ(scn::detail::decode_utf8_code_point_exhaustive("😀"),
              char32_t{0x1f600});
    EXPECT_EQ(scn::detail::decode_utf8_code_point_exhaustive_valid("😀"),
              char32_t{0x1f600});
}

TEST(UnicodeTest, Utf16CpLength)
{
    EXPECT_EQ(scn::detail::utf16_code_point_length_by_starting_code_unit(
                  uint16_t{'a'}),
              1);
    EXPECT_EQ(scn::detail::utf16_code_point_length_by_starting_code_unit(
                  uint16_t{0xe4}),
              1);
    EXPECT_EQ(scn::detail::utf16_code_point_length_by_starting_code_unit(
                  uint16_t{0xdaaa}),
              2);
    EXPECT_EQ(scn::detail::utf16_code_point_length_by_starting_code_unit(
                  uint16_t{0xdeee}),
              0);
}
