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

#include "fuzz.h"

template <typename T, typename Result>
void check_roundtrip(const T& value, const T& original, Result& result)
{
    if (!result) {
        throw std::runtime_error("Failed to read");
    }
    if (value != original) {
        throw std::runtime_error("Roundtrip failure");
    }
    if (!result.range().empty()) {
        if (!result.range().is_direct) {
            if (result.range().size() == 1) {
                auto e = scn::read_code_unit(result.range(), false);
                if (!e && e.error() == scn::error::end_of_range) {
                    return;
                }
            }
        }
        throw std::runtime_error("Unparsed input");
    }
}

template <typename CharT, typename T, typename Source>
void do_roundtrip(const T& original_value, const Source& s)
{
    {
        T value{};
        auto result = scn::scan(s, default_format_string<CharT>::value, value);
        check_roundtrip(value, original_value, result);
    }
    {
        T value{};
        auto result = scn::scan_default(s, value);
        check_roundtrip(value, original_value, result);
    }
    {
        T value{};
        auto result = scn::scan_localized(
            global_locale, s, default_format_string<CharT>::value, value);
        check_roundtrip(value, original_value, result);
    }
}

template <typename CharT, typename T, typename Source>
void roundtrip_for_type(Source data)
{
    if (data.size() < sizeof(T)) {
        return;
    }
    T original_value{};
    std::memcpy(&original_value, data.data(), sizeof(T));

    std::basic_ostringstream<CharT> oss;
    oss << original_value;

    auto source_str = SCN_MOVE(oss).str();
    do_roundtrip<CharT>(original_value, source_str);

    auto source_sv =
        scn::basic_string_view<CharT>(source_str.data(), source_str.size());
    do_roundtrip<CharT>(original_value, source_sv);

    auto& source_deque = populate_deque(source_sv);
    do_roundtrip<CharT>(original_value, source_deque);

    auto& source_indirect = populate_indirect(source_sv);
    do_roundtrip<CharT>(original_value, source_indirect);
    reset_indirect(SCN_MOVE(source_indirect));
}

template <typename Source>
void roundtrip_for_source(Source source)
{
    using char_type = typename Source::value_type;

    roundtrip_for_type<char_type, short>(source);
    roundtrip_for_type<char_type, int>(source);
    roundtrip_for_type<char_type, long>(source);
    roundtrip_for_type<char_type, long long>(source);
    roundtrip_for_type<char_type, unsigned short>(source);
    roundtrip_for_type<char_type, unsigned int>(source);
    roundtrip_for_type<char_type, unsigned long>(source);
    roundtrip_for_type<char_type, unsigned long long>(source);
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    if (size > max_input_bytes || size == 0) {
        return 0;
    }

    scn::string_view sv;
    scn::wstring_view wsv1, wsv2;
    populate_views(data, size, sv, wsv1, wsv2);

    roundtrip_for_source(sv);
    roundtrip_for_source(wsv1);
    roundtrip_for_source(wsv2);

    return 0;
}
