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

/*
Copyright (c) 2017-2024,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance
for Sustainable Energy, LLC.  See the top-level NOTICE for additional details.
All rights reserved. SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include "charMapper.h"
#include "stringOps.h"

#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

namespace gmlc {
namespace utilities {
    extern const CharMapper<bool> numCheck;
    extern const CharMapper<bool> numCheckEnd;

    // templates for single numerical conversion
    template<typename X>
    inline X numConv(const std::string& V)
    {
        return (std::is_integral<X>::value) ? X(numConv<int64_t>(V)) :
                                              X(numConv<double>(V));
    }

    // template definition for double conversion
    template<>
    inline double numConv(const std::string& V)
    {
        return std::stod(V);
    }

    // template definition for long double conversion
    template<>
    inline long double numConv(const std::string& V)
    {
        return std::stold(V);
    }

    // template definition for integer conversion
    template<>
    inline int numConv(const std::string& V)
    {
        return std::stoi(V);
    }

    // template definition for unsigned long conversion
    template<>
    inline uint32_t numConv(const std::string& V)
    {
        return std::stoul(V);
    }
    // template definition for unsigned long long conversion
    template<>
    inline uint64_t numConv(const std::string& V)
    {
        return std::stoull(V);
    }

    // template definition for long long conversion
    template<>
    inline int64_t numConv(const std::string& V)
    {
        return std::stoll(V);
    }

    // template for numeric conversion returning the position
    template<class X>
    inline X numConvComp(const std::string& V, size_t& rem)
    {
        return (std::is_integral<X>::value) ? X(numConvComp<int64_t>(V, rem)) :
                                              X(numConvComp<double>(V, rem));
    }

    template<>
    inline float numConvComp(const std::string& V, size_t& rem)
    {
        return std::stof(V, &rem);
    }

    template<>
    inline double numConvComp(const std::string& V, size_t& rem)
    {
        return std::stod(V, &rem);
    }

    template<>
    inline long double numConvComp(const std::string& V, size_t& rem)
    {
        return std::stold(V, &rem);
    }

    template<>
    inline int numConvComp(const std::string& V, size_t& rem)
    {
        return std::stoi(V, &rem);
    }

    template<>
    inline uint32_t numConvComp(const std::string& V, size_t& rem)
    {
        return std::stoul(V, &rem);
    }

    template<>
    inline uint64_t numConvComp(const std::string& V, size_t& rem)
    {
        return std::stoull(V, &rem);
    }

    template<>
    inline int64_t numConvComp(const std::string& V, size_t& rem)
    {
        return std::stoll(V, &rem);
    }

    /** check if the first character of the string is a valid numerical value*/
    inline bool nonNumericFirstCharacter(const std::string& V)
    {
        return ((V.empty()) || (!numCheck[V[0]]));
    }

    /** check if the first character of the string is a valid numerical value*/
    inline bool nonNumericFirstOrLastCharacter(const std::string& V)
    {
        return ((V.empty()) || (!numCheck[V[0]]) || (!numCheckEnd[V.back()]));
    }

    template<typename X>
    X numeric_conversion(const std::string& V, const X defValue)
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
    X numeric_conversionComplete(const std::string& V, const X defValue)
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
        const std::string& line,
        const X defValue,
        const std::string& delimiters = ",;")
    {
        auto tempVec = stringOps::splitline(line, delimiters);
        std::vector<X> av;
        av.reserve(tempVec.size());
        for (const auto& str : tempVec) {
            av.push_back(numeric_conversion<X>(str, defValue));
        }
        return av;
    }

    /** @brief  convert a vector of strings into doubles
@param[in] tokens the vector of string to convert
@param[in] defValue  the default numerical return value if conversion fails
@return a vector of double precision numbers converted from the string
*/
    template<typename X>
    std::vector<X> str2vector(const stringVector& tokens, const X defValue)
    {
        std::vector<X> av;
        av.reserve(tokens.size());
        for (const auto& str : tokens) {
            av.push_back(numeric_conversion<X>(str, defValue));
        }
        return av;
    }

}  // namespace utilities
}  // namespace gmlc
