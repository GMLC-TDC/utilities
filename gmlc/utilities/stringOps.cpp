/*
 * LLNS Copyright Start
 * Copyright (c) 2017, Lawrence Livermore National Security
 * This work was performed under the auspices of the U.S. Department
 * of Energy by Lawrence Livermore National Laboratory in part under
 * Contract W-7405-Eng-48 and in part under Contract DE-AC52-07NA27344.
 * Produced at the Lawrence Livermore National Laboratory.
 * All rights reserved.
 * For details, see the LICENSE file.
 * LLNS Copyright End
 */

/*
Copyright (c) 2017-2022,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance
for Sustainable Energy, LLC.  See the top-level NOTICE for additional details.
All rights reserved. SPDX-License-Identifier: BSD-3-Clause
*/

/** changelog
June 2022 changed to support string view where applicable
*/
#include "stringOps.h"

#include "generic_string_ops.hpp"

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <random>
#include <utility>

namespace gmlc::utilities {
static auto lower = [](char x) -> char {
    return (x >= 'A' && x <= 'Z') ? (x - ('Z' - 'z')) : x;
};

static auto upper = [](char x) -> char {
    return (x >= 'a' && x <= 'z') ? (x + ('Z' - 'z')) : x;
};

std::string convertToLowerCase(std::string_view input)
{
    std::string out(input);
    std::transform(out.begin(), out.end(), out.begin(), lower);
    return out;
}

std::string convertToUpperCase(std::string_view input)
{
    std::string out(input);
    std::transform(out.begin(), out.end(), out.begin(), upper);
    return out;
}

void makeLowerCase(std::string& input)
{
    std::transform(input.begin(), input.end(), input.begin(), lower);
}

void makeUpperCase(std::string& input)
{
    std::transform(input.begin(), input.end(), input.begin(), upper);
}

namespace stringOps {
    stringVector splitline(
        std::string_view line,
        std::string_view delimiters,
        delimiter_compression compression)
    {
        return generalized_string_split<std::string_view, std::string>(
            line, delimiters, (compression == delimiter_compression::on));
    }

    stringVector splitline(std::string_view line, char del)
    {
        return generalized_string_split<std::string_view, std::string>(
            line, std::string_view{&del, 1}, false);
    }

    void splitline(
        std::string_view line,
        stringVector& strVec,
        std::string_view delimiters,
        delimiter_compression compression)
    {
        strVec = generalized_string_split<std::string_view, std::string>(
            line, delimiters, (compression == delimiter_compression::on));
    }

    void splitline(std::string_view line, stringVector& strVec, char del)
    {
        strVec = generalized_string_split<std::string_view, std::string>(
            line, std::string_view{&del, 1}, false);
    }

    static const auto pmap = pairMapper();

    stringVector splitlineQuotes(
        std::string_view line,
        std::string_view delimiters,
        std::string_view quoteChars,
        delimiter_compression compression)
    {
        bool compress = (compression == delimiter_compression::on);
        return generalized_section_splitting<std::string_view, std::string>(
            line, delimiters, quoteChars, pmap, compress);
    }

    stringVector splitlineBracket(
        std::string_view line,
        std::string_view delimiters,
        std::string_view bracketChars,
        delimiter_compression compression)
    {
        bool compress = (compression == delimiter_compression::on);
        return generalized_section_splitting<std::string_view, std::string>(
            line, delimiters, bracketChars, pmap, compress);
    }

    void trimString(std::string& input, std::string_view whitespace)
    {
        input.erase(input.find_last_not_of(whitespace) + 1);
        input.erase(0, input.find_first_not_of(whitespace));
    }

    std::string trim(std::string_view input, std::string_view whitespace)
    {
        const auto strStart = input.find_first_not_of(whitespace);
        if (strStart == std::string::npos) {
            return std::string();  // no content
        }

        const auto strEnd = input.find_last_not_of(whitespace);

        return std::string{input.substr(strStart, strEnd - strStart + 1)};
    }

    void trim(stringVector& input, std::string_view whitespace)
    {
        for (auto& str : input) {
            trimString(str, whitespace);
        }
    }

    static constexpr std::string_view digits("0123456789");
    int trailingStringInt(
        std::string_view input,
        std::string& output,
        int defNum) noexcept
    {
        if ((input.empty()) || (isdigit(input.back()) == 0)) {
            output = input;
            return defNum;
        }
        int num = defNum;
        auto pos1 = input.find_last_not_of(digits);
        if (pos1 == std::string::npos)  // in case the whole thing is a number
        {
            if (input.length() <= 10) {
                output.clear();
                std::from_chars(input.data(), input.data() + input.size(), num);
                return num;
            }
            pos1 = input.length() - 10;
        }

        size_t length = input.length();
        if (pos1 == length - 2) {
            num = input.back() - '0';
        } else if (length <= 10 || pos1 >= length - 10) {
            auto sub = input.substr(pos1 + 1);
            std::from_chars(sub.data(), sub.data() + sub.size(), num);
        } else {
            auto sub = input.substr(length - 9);
            std::from_chars(sub.data(), sub.data() + sub.size(), num);
            pos1 = length - 10;
        }

        if (input[pos1] == '_' || input[pos1] == '#') {
            output = input.substr(0, pos1);
        } else {
            output = input.substr(0, pos1 + 1);
        }

        return num;
    }

    int trailingStringInt(std::string_view input, int defNum) noexcept
    {
        if ((input.empty()) || (isdigit(input.back()) == 0)) {
            return defNum;
        }

        auto pos1 = input.find_last_not_of(digits);
        int num{0};
        if (pos1 == std::string::npos)  // in case the whole thing is a number
        {
            if (input.length() <= 10) {
                std::from_chars(input.data(), input.data() + input.size(), num);
                return num;
            }
            pos1 = input.length() - 10;
        }

        size_t length = input.length();
        if (pos1 == length - 2) {
            return input.back() - '0';
        }
        if ((length <= 10) || (pos1 >= length - 10)) {
            auto sub = input.substr(pos1 + 1);

            std::from_chars(sub.data(), sub.data() + sub.size(), num);
            return num;
        }
        auto sub = input.substr(length - 9);

        std::from_chars(sub.data(), sub.data() + sub.size(), num);
        return num;
    }

    std::string removeQuotes(std::string_view str)
    {
        auto newString = trim(str);
        if (!newString.empty()) {
            if ((newString.front() == '\"') || (newString.front() == '\'') ||
                (newString.front() == '`')) {
                if (newString.back() == newString.front()) {
                    newString.pop_back();
                    newString.erase(0, 1);
                }
            }
        }
        return newString;
    }

    std::string removeBrackets(std::string_view str)
    {
        std::string newString = trim(str);
        if (!newString.empty()) {
            if ((newString.front() == '[') || (newString.front() == '(') ||
                (newString.front() == '{') || (newString.front() == '<')) {
                if (newString.back() == pmap[newString.front()]) {
                    newString.pop_back();
                    newString.erase(0, 1);
                }
            }
        }
        return newString;
    }

    std::string getTailString(std::string_view input, char sep) noexcept
    {
        auto tc = input.find_last_of(sep);
        auto ret = std::string(
            (tc == std::string::npos) ? input : input.substr(tc + 1));
        return ret;
    }

    std::string
        getTailString(std::string_view input, std::string_view sep) noexcept
    {
        auto tc = input.rfind(sep);
        std::string_view ret = (tc == std::string_view::npos) ?
            input :
            input.substr(tc + sep.size());
        return std::string(ret);
    }

    std::string
        getTailString_any(std::string_view input, std::string_view sep) noexcept
    {
        auto tc = input.find_last_of(sep);
        auto ret = std::string(
            (tc == std::string::npos) ? input : input.substr(tc + 1));
        return ret;
    }

    int findCloseStringMatch(
        const stringVector& testStrings,
        const stringVector& iStrings,
        string_match_type matchType)
    {
        std::string lct;  // lower case test string
        std::string lcis;  // lower case input string
        stringVector lciStrings = iStrings;
        // make all the input strings lower case
        for (auto& st : lciStrings) {
            makeLowerCase(st);
        }
        for (auto& ts : testStrings) {
            lct = convertToLowerCase(ts);
            for (int kk = 0; kk < static_cast<int>(lciStrings.size()); ++kk) {
                lcis = lciStrings[kk];
                switch (matchType) {
                    case string_match_type::exact:
                        if (lcis == lct) {
                            return kk;
                        }
                        break;
                    case string_match_type::begin:
                        if (lcis.compare(0, lct.length(), lct) == 0) {
                            return kk;
                        }
                        break;
                    case string_match_type::end:
                        if (lct.length() > lcis.length()) {
                            continue;
                        }
                        if (lcis.compare(
                                lcis.length() - lct.length(),
                                lct.length(),
                                lct) == 0) {
                            return kk;
                        }
                        break;
                    case string_match_type::close:
                        if (lct.length() == 1)  // special case
                        {  // we are checking if the single character is
                           // isolated from
                            // other other alphanumeric characters
                            auto bf = lcis.find(lct);
                            while (bf != std::string::npos) {
                                if (bf == 0) {
                                    if ((isspace(lcis[bf + 1]) != 0) ||
                                        (ispunct(lcis[bf + 1]) != 0)) {
                                        return kk;
                                    }
                                } else if (bf == lcis.length() - 1) {
                                    if ((isspace(lcis[bf - 1]) != 0) ||
                                        (ispunct(lcis[bf - 1]) != 0)) {
                                        return kk;
                                    }
                                } else {
                                    if ((isspace(lcis[bf - 1]) != 0) ||
                                        (ispunct(lcis[bf - 1]) != 0)) {
                                        if ((isspace(lcis[bf + 1]) != 0) ||
                                            (ispunct(lcis[bf + 1]) != 0)) {
                                            return kk;
                                        }
                                    }
                                }
                                bf = lcis.find(lct, bf + 1);
                            }
                        } else {
                            auto bf = lcis.find(lct);
                            if (bf != std::string::npos) {
                                return kk;
                            }
                            auto nstr = removeChar(lct, '_');
                            if (lcis == nstr) {
                                return kk;
                            }
                            auto nstr2 = removeChar(lcis, '_');
                            if ((lct == nstr2) || (nstr == nstr2)) {
                                return kk;
                            }
                        }
                        break;
                }
            }
        }
        return -1;
    }

    std::string removeChars(std::string_view source, std::string_view remchars)
    {
        std::string result;
        result.reserve(source.length());
        std::remove_copy_if(
            source.begin(),
            source.end(),
            std::back_inserter(result),
            [remchars](char in) {
                return (
                    std::find(remchars.begin(), remchars.end(), in) !=
                    remchars.end());
            });
        return result;
    }

    std::string removeChar(std::string_view source, char remchar)
    {
        std::string result;
        result.reserve(source.length());
        std::remove_copy(
            source.begin(), source.end(), std::back_inserter(result), remchar);
        return result;
    }

    std::string characterReplace(
        std::string_view source,
        char key,
        std::string_view repStr)
    {
        std::string result;
        result.reserve(source.length());
        for (auto sc : source) {
            if (sc == key) {
                result += repStr;
            } else {
                result.push_back(sc);
            }
        }
        return result;
    }

    std::string xmlCharacterCodeReplace(std::string str)
    {
        std::string out = std::move(str);
        auto tt = out.find("&gt;");
        while (tt != std::string::npos) {
            out.replace(tt, 4, ">");
            tt = out.find("&gt;", tt + 1);
        }
        tt = out.find("&lt;");
        while (tt != std::string::npos) {
            out.replace(tt, 4, "<");
            tt = out.find("&lt;", tt + 1);
        }
        tt = out.find("&quot;");
        while (tt != std::string::npos) {
            out.replace(tt, 6, "\"");
            tt = out.find("&quot;", tt + 1);
        }
        tt = out.find("&apos;");
        while (tt != std::string::npos) {
            out.replace(tt, 6, "'");
            tt = out.find("&apos;", tt + 1);
        }
        // &amp; is last so it can't trigger other sequences
        tt = out.find("&amp;");
        while (tt != std::string::npos) {
            out.replace(tt, 5, "&");
            tt = out.find("&amp;", tt + 1);
        }
        return out;
    }
}  // namespace stringOps

std::string randomString(std::string::size_type length)
{
    static constexpr auto chars =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    thread_local static std::mt19937 rg{
        std::random_device{}() +
        static_cast<uint32_t>(
            reinterpret_cast<uint64_t>(&length) &  // NOLINT
            0xFFFFFFFFU)};
    thread_local static std::uniform_int_distribution<std::string::size_type>
        pick(0, 61);
    std::string s;

    s.reserve(length);

    while (length-- != 0U) {
        s.push_back(chars[pick(rg)]);
    }

    return s;
}

}  // namespace gmlc::utilities
