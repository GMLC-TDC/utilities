/*
 * LLNS Copyright Start
 * Copyright (c) 2014-2018, Lawrence Livermore National Security
 * This work was performed under the auspices of the U.S. Department
 * of Energy by Lawrence Livermore National Laboratory in part under
 * Contract W-7405-Eng-48 and in part under Contract DE-AC52-07NA27344.
 * Produced at the Lawrence Livermore National Laboratory.
 * All rights reserved.
 * For details, see the LICENSE file.
 * LLNS Copyright End
 */
#pragma once

#include "charMapper.h"
#include "string_viewOps.h"

#include <charconv>
#ifndef __cpp_lib_to_chars
#if defined USE_BOOST_SPIRIT && USE_BOOST_SPIRIT > 0
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4127 4459)
#endif
#include "boost/spirit/home/x3.hpp"

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
#endif

#include <stdexcept>
#include <string>
#include <vector>

namespace gmlc::utilities {
extern const CharMapper<bool> numCheck;
extern const CharMapper<bool> numCheckEnd;

template<typename X>
X strViewToInteger(std::string_view input, size_t* charactersUsed = nullptr)
{
    static_assert(std::is_integral_v<X>, "requested type is not integral");
    X val{0};
    if (charactersUsed != nullptr) {
        *charactersUsed = 0;
    }
    std::size_t additionalChars{0};
    if (input.size() > 1) {
        while (input[0] == ' ') {
            input.remove_prefix(1);
            ++additionalChars;
            if (input.empty()) {
                if (charactersUsed != nullptr) {
                    *charactersUsed = additionalChars;
                }
                return val;
            }
        }
        if (input.front() == '0') {
            if (input[1] != 'X' && input[1] != 'x') {
                while (input[0] == '0') {
                    input.remove_prefix(1);
                    ++additionalChars;
                    if (input.empty()) {
                        if (charactersUsed != nullptr) {
                            *charactersUsed = additionalChars;
                        }
                        return val;
                    }
                }
            }
        }
    }
    auto conversionResult =
        std::from_chars(input.data(), input.data() + input.size(), val);
    if (conversionResult.ec == std::errc()) {
        if (charactersUsed != nullptr) {
            *charactersUsed =
                (conversionResult.ptr - input.data()) + additionalChars;
        }
        return val;
    }
    if (conversionResult.ec == std::errc::result_out_of_range) {
        if (charactersUsed != nullptr) {
            *charactersUsed = (conversionResult.ptr - input.data());
        }
        throw(std::out_of_range(
            "conversion type does not support the string conversion"));
    }
    if constexpr (std::is_unsigned_v<X>) {
        if (input.size() > 1 && input.front() == '-') {
            auto signedVal =
                strViewToInteger<std::make_signed_t<X>>(input, charactersUsed);
            // additional chars must be 0 in this case otherwise it should have
            // been an error
            return static_cast<X>(signedVal);
        }
    }
    throw(std::invalid_argument("unable to convert string"));
}

#ifdef __cpp_lib_to_chars
template<typename X>
X strViewToFloat(std::string_view input, size_t* charactersUsed = nullptr)
{
    static_assert(
        std::is_floating_point_v<X>, "requested type is not floating point");
    X val{0};
    if (charactersUsed != nullptr) {
        *charactersUsed = 0;
    }
    auto conversionResult =
        std::from_chars(input.data(), input.data() + input.size(), val);
    if (conversionResult.ec == std::errc{}) {
        if (charactersUsed != nullptr) {
            *charactersUsed = (conversionResult.ptr - input.data());
        }
        return val;
    }
    if (conversionResult.ec == std::errc::result_out_of_range) {
        if (charactersUsed != nullptr) {
            *charactersUsed = (conversionResult.ptr - input.data());
        }
        throw(std::out_of_range(
            "conversion type does not support the string conversion"));
    }
    throw(std::invalid_argument("unable to convert string"));
}
#endif

// templates for single numerical conversion
template<typename X>
inline X numConv(std::string_view V)
{
    return (std::is_integral<X>::value) ? strViewToInteger<X>(V) :
                                          X(numConv<double>(V));
}

// template definition for double conversion
template<>
inline double numConv(std::string_view V)
{
#ifdef __cpp_lib_to_chars
    return strViewToFloat<double>(V);
#elif defined USE_BOOST_SPIRIT && USE_BOOST_SPIRIT > 0
    namespace x3 = boost::spirit::x3;
    double retVal = -1e49;
    x3::parse(V.cbegin(), V.cend(), x3::double_, retVal);
    return retVal;
#else
    return std::stod(std::string(V.data(), V.length()));
#endif
}

template<>
inline float numConv(std::string_view V)
{
#ifdef __cpp_lib_to_chars
    return strViewToFloat<float>(V);
#elif defined USE_BOOST_SPIRIT && USE_BOOST_SPIRIT > 0
    namespace x3 = boost::spirit::x3;
    float retVal = -1e25f;
    x3::parse(V.cbegin(), V.cend(), x3::float_, retVal);
    return retVal;
#else
    return std::stof(std::string(V.data(), V.length()));
#endif
}

// template definition for long double conversion
template<>
inline long double numConv(std::string_view V)
{
#ifdef __cpp_lib_to_chars
    return strViewToFloat<long double>(V);
#else
    return std::stold(std::string(V.data(), V.length()));
#endif
}

// template for numeric conversion returning the position
template<class X>
inline X numConvComp(std::string_view V, size_t& charactersUsed)
{
    return (std::is_integral<X>::value) ?
        strViewToInteger<X>(V, &charactersUsed) :
        X(numConvComp<double>(V, charactersUsed));
}

template<>
inline float numConvComp(std::string_view V, size_t& charactersUsed)
{
#ifdef __cpp_lib_to_chars
    return strViewToFloat<float>(V, &charactersUsed);
#else
    return std::stof(std::string(V.data(), V.length()), &charactersUsed);
#endif
}

template<>
inline double numConvComp(std::string_view V, size_t& charactersUsed)
{
#ifdef __cpp_lib_to_chars
    return strViewToFloat<double>(V, &charactersUsed);
#else
    return std::stod(std::string(V.data(), V.length()), &charactersUsed);
#endif
}

template<>
inline long double numConvComp(std::string_view V, size_t& charactersUsed)
{
#ifdef __cpp_lib_to_chars
    return strViewToFloat<long double>(V, &charactersUsed);
#else
    return std::stold(std::string(V.data(), V.length()), &charactersUsed);
#endif
}

/** check if the first character of the string is a valid numerical value*/
inline bool nonNumericFirstCharacter(std::string_view V)
{
    return ((V.empty()) || (numCheck[V[0]] == false));
}

/** check if the first character of the string is a valid numerical value*/
inline bool nonNumericFirstOrLastCharacter(std::string_view V)
{
    return (
        (V.empty()) || (numCheck[V[0]] == false) ||
        (numCheckEnd[V.back()] == false));
}

template<typename X>
X numeric_conversion(std::string_view V, const X defValue)
{
    if (nonNumericFirstCharacter(V)) {
        return defValue;
    }
    try {
        return numConv<X>(V);
    }
    catch (const std::invalid_argument&) {
        return defValue;
    }
}

/** do a numeric conversion of the complete string
 */
template<typename X>
X numeric_conversionComplete(std::string_view V, const X defValue)
{
    if (nonNumericFirstOrLastCharacter(V)) {
        return defValue;
    }
    try {
        size_t rem;
        X res = numConvComp<X>(V, rem);
        while (rem < V.length()) {
            if (!(isspace(V[rem]))) {
                res = defValue;
                break;
            }
            ++rem;
        }
        return res;
    }
    catch (const std::invalid_argument&) {
        return defValue;
    }
}

/** @brief  convert a string into a vector of double precision numbers
@param[in] line the string to convert
@param[in] defValue  the default numerical return value if conversion fails
@param[in] delimiters  the delimiters to use to separate the numbers
@return a vector of double precision numbers converted from the string
*/
template<typename X>
std::vector<X> str2vector(
    std::string_view line,
    const X defValue,
    std::string_view delimiters = string_viewOps::default_delim_chars)
{
    line = utilities::string_viewOps::removeBrackets(line);
    auto tempVec = utilities::string_viewOps::split(line, delimiters);
    std::vector<X> av;
    av.reserve(tempVec.size());
    for (const auto& str : tempVec) {
        av.push_back(numeric_conversion<X>(str, defValue));
    }
    return av;
}

/** @brief  convert a vector of strViews into doubles
@param[in] tokens the vector of strViews to convert
@param[in] defValue  the default numerical return value if conversion fails
@return a vector of double precision numbers converted from the string
*/
template<typename X>
std::vector<X> str2vector(const string_viewVector& tokens, const X defValue)
{
    std::vector<X> av;
    av.reserve(tokens.size());
    for (const auto& str : tokens) {
        av.push_back(numeric_conversion<X>(str, defValue));
    }
    return av;
}

}  // namespace gmlc::utilities
