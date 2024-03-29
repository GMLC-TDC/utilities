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
Copyright (c) 2017-2024,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance
for Sustainable Energy, LLC.  See the top-level NOTICE for additional details.
All rights reserved. SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <array>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace gmlc::utilities {
//!< alias for convenience
using stringVector = std::vector<std::string>;

/* some common functions that don't need to be in the namespace*/

/** @brief convert a string to lower case as a new string
@param input  the string to convert
@return the string with all upper case converted to lower case
*/
std::string convertToLowerCase(std::string_view input);
/** @brief convert a string to upper case as a new string
@param input  the string to convert
@return the string with all lower case letters converted to upper case
*/
std::string convertToUpperCase(std::string_view input);
/** @brief make a string lower case
@param[in,out] input  the string to convert
*/
void makeLowerCase(std::string& input);

/** @brief make a string upper case
@param[in,out] input  the string to convert
*/
void makeUpperCase(std::string& input);

namespace stringOps {
    constexpr unsigned int factors[] = {
        1,
        10,
        100,
        1'000,
        10'000,
        100'000,
        1'000'000,
        10'000'000,
        100'000'000};
    /**@brief append the text of the integral part of a number to a string*/
    template<typename X>
    void appendInteger(std::string& input, X val)
    {
        if (val < X(0)) {
            input.push_back('-');
        }
        X x = (std::is_signed<X>::value) ? ((val < X(0)) ? (X(0) - val) : val) :
                                           val;
        if (x < 10) {
            input.push_back(static_cast<char>(x + '0'));
            return;
        }
        int digits =
            (x < 100 ?
                 2 :
                 (x < 1000 ?
                      3 :
                      (x < 10'000 ?
                           4 :
                           (x < 100'000 ?
                                5 :
                                (x < 1'000'000 ?
                                     6 :
                                     (x < 10'000'000 ?
                                          7 :
                                          (x < 100'000'000 ?
                                               8 :
                                               (x < 1'000'000'000 ? 9 :
                                                                    500))))))));
        if (digits > 9)  // don't deal with really big numbers
        {
            input += std::to_string(x);
            return;
        }
        unsigned int rem = static_cast<int>(x);
        for (auto dig = digits - 1; dig >= 0; --dig) {
            unsigned int place = rem / factors[dig];
            input.push_back(static_cast<char>(place + '0'));
            rem -= factors[dig] * place;
        }
    }
    constexpr std::array<char, 8>
        whiteChars{' ', '\t', '\n', '\r', '\a', '\v', '\f', '\0'};
    constexpr std::string_view
        whiteSpaceCharacters(whiteChars.data(), whiteChars.size());

    /** @brief trim characters (default=whitespace) from a string at the
beginning and end of the string
@param[in,out] input  the string to convert
*/
    void trimString(
        std::string& input,
        std::string_view whitespace = whiteSpaceCharacters);

    /** @brief trim characters (default=whitespace) from a string
@param input the string to trim;
@param whitespace  the set of characters to trim"
@return the trimmed string
*/

    std::string trim(
        std::string_view input,
        std::string_view whitespace = whiteSpaceCharacters);

    /** @brief trim characters (default=whitespace) from a vector of strings
@param input the vector of strings to trim;
@param whitespace  the definition of characters to trim
*/

    void trim(
        stringVector& input,
        std::string_view whitespace = whiteSpaceCharacters);

    /** @brief get a string that comes after the last of a specified
separator
@param input  the string to separate
@param sep the separation character
@return  the tail string or the string that comes after the last sep character
if not found returns the entire string
*/
    std::string getTailString(std::string_view input, char sep) noexcept;
    /** @brief get a string that comes after the last of a specified
separator
@param input  the string to separate
@param sep the separation string (single character or sequence of characters
@return  the tail string or the string that comes after the last of the
separation strings; if not found returns the entire string
*/
    std::string
        getTailString(std::string_view input, std::string_view sep) noexcept;

    /** @brief get a string that comes after the last of any specified
separator
@param input  the string to separate
@param sep the set of separation characters
@return  the tail string or the string that comes after the last of the
separation strings; if not found returns the entire string
*/
    std::string getTailString_any(
        std::string_view input,
        std::string_view sep) noexcept;

    constexpr std::string_view default_delim_chars(",;");
    constexpr std::string_view default_quote_chars(R"raw('"`)raw");
    constexpr std::string_view default_bracket_chars(R"raw([{(<'"`)raw");

    enum class delimiter_compression {
        on,
        off,
    };

    /** @brief split a line into a vector of strings
@param line  the string to spit
@param  delimiters a string containing the valid delimiter characters
@param compression default off,  if set to delimiter_compression::on will merge
multiple sequential delimiters together
@return a vector of strings separated by the delimiters characters
*/
    stringVector splitline(
        std::string_view line,
        std::string_view delimiters = default_delim_chars,
        delimiter_compression compression = delimiter_compression::off);

    /** @brief split a line into a vector of strings
@param line  the string to spit
@param del the delimiter character
@return a vector of strings separated by the delimiters characters
*/
    stringVector splitline(std::string_view line, char del);

    /** @brief split a line into a vector of strings
@param line  the string to spit
@param[out] strVec vector to place the strings
@param del the delimiter character

*/
    void splitline(std::string_view line, stringVector& strVec, char del);

    /** @brief split a line into a vector of strings
@param line  the string to spit
@param[out] strVec vector to place the strings
@param  delimiters a string containing the valid delimiter characters
@param compression default off,  if set to delimiter_compression::on will merge
multiple sequential delimiters together
*/
    void splitline(
        std::string_view line,
        stringVector& strVec,
        std::string_view delimiters = default_delim_chars,
        delimiter_compression compression = delimiter_compression::off);

    /** @brief split a line into a vector of strings taking into account
quote characters the delimiter characters are allowed inside the brackets and
the resulting vector will take the brackets into account
@param line  the string to split
@param  delimiters a string containing the valid delimiter characters
@param compression default off,  if set to delimiter_compression::on will merge
multiple sequential delimiters together
@return a vector of strings separated by the delimiters characters accounting
for bracketing characters
*/
    stringVector splitlineQuotes(
        std::string_view line,
        std::string_view delimiters = default_delim_chars,
        std::string_view quoteChars = default_quote_chars,
        delimiter_compression compression = delimiter_compression::off);

    /** @brief split a line into a vector of strings taking into account
bracketing characters bracket characters include "()","{}","[]","<>" as well as
quote characters ' and " the delimiter characters are allowed inside the
brackets and the resulting vector will take the brackets into account
@param line  the string to spit
@param  delimiters a string containing the valid delimiter characters
@param bracketChars a string of characters define what valid bracket characters
are
@param compression default off,  if set to delimiter_compression::on will merge
multiple sequential delimiters together
@return a vector of strings separated by the delimiters characters accounting
for bracketing characters
*/
    stringVector splitlineBracket(
        std::string_view line,
        std::string_view delimiters = default_delim_chars,
        std::string_view bracketChars = default_bracket_chars,
        delimiter_compression compression = delimiter_compression::off);

    /** @brief extract a trailing number from a string return the number and
the string without the number
@details will convert up to the last 9 digits so there is no overload there is
no throw technically could if the string allocation were to fail but that is a
strange case
@param input the string to extract the information from
@param[out] output the leading string with the numbers removed
@param defNum the default number to return if no trailing number was found
@return the numerical value of the trailing number*/
    int trailingStringInt(
        std::string_view input,
        std::string& output,
        int defNum = -1) noexcept;

    /** @brief extract a trailing number from a string
@details will convert up to the last 9 digits so there is no overload there is
no throw technically could if the string allocation were to fail but that is a
strange case
@param input the string to extract the information from
@param defNum the default number to return if no trailing number was found
@return the numerical value of the trailing number*/
    int trailingStringInt(std::string_view input, int defNum = -1) noexcept;

    /**@brief enumeration for string close matches
     */
    enum class string_match_type {
        close,  //!< the middle of one string matches another, ignoring '_'
        begin,  //!< the beginning of one string matches another
        end,  //!< the ends of one string matches another
        exact  //!< an exact match
    };

    /** @brief find a close match in a vector of strings to a test string
function searches for any of the testStrings in the testStrings vector based on
the matchType parameter and returns the index into the testStrings vector
@param testStrings the vector of strings to search for
@param inputStrings the string library to search through
@param matchType the matching type
@return the index of the match or -1 if no match is found
*/
    int findCloseStringMatch(
        const stringVector& testStrings,
        const stringVector& inputStrings,
        string_match_type matchType = string_match_type::close);

    /** @brief remove a set of characters from a string
@param source  the original string
@param remchars the characters to remove
@return  the string with the specified characters removed
*/
    std::string removeChars(std::string_view source, std::string_view remchars);

    /** @brief remove a particular character from a string
@param source  the original string
@param remchar the character to remove
@return  the string with the specified character removed
*/
    std::string removeChar(std::string_view source, char remchar);

    /** @brief remove quotes from a string
only quotes around the edges are removed along with whitespace outside the
quotes
@param str the original string
@return  the string with quotes removed
*/
    std::string removeQuotes(std::string_view str);

    /** @brief outer brackets from a string
Bracket characters include [({<
@param str  the original string
@return  the string with brackets removed
*/
    std::string removeBrackets(std::string_view str);

    /** @brief replace a particular key character with a different string
@param source  the original string
@param key the character to replace
@param repStr the string to replace the key with
@return  the string after the specified replacement
*/
    std::string characterReplace(
        std::string_view source,
        char key,
        std::string_view repStr);

    /** @brief replace XML character codes with the appropriate character
@param str  the string to do the replacement on
@return the string with the character codes removed and replaced with the
appropriate character
*/
    std::string xmlCharacterCodeReplace(std::string str);
}  // namespace stringOps
/** brief generate a random string of a specific length
@details the randomly generated string will contain characters from a-zA-Z0-9
@return a randomly generated string*/
std::string randomString(std::string::size_type length);

}  // namespace gmlc::utilities
