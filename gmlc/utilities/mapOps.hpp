/*
Copyright (c) 2017-2022,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance
for Sustainable Energy, LLC.  See the top-level NOTICE for additional details.
All rights reserved. SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <optional>

#include <map>
#include <unordered_map>

template<class X1, class X2>
inline X2 mapFind(const std::map<X1, X2>& mapS, const X1& val, const X2& defVal)
{
    auto map_it = mapS.find(val);
    return (map_it != mapS.end()) ? map_it->second : defVal;
}

template<class X1, class X2>
std::optional<X2> mapFind(const std::map<X1, X2>& mapS, const X1& val)
{
    auto map_it = mapS.find(val);
    if (map_it != mapS.end()) {
        return map_it->second;
    }
    return {};
}

template<class X1, class X2>
inline X2 mapFind(
    const std::unordered_map<X1, X2>& mapS,
    const X1& val,
    const X2& defVal)
{
    auto map_it = mapS.find(val);
    return (map_it != mapS.end()) ? map_it->second : defVal;
}

template<class X1, class X2>
std::optional<X2>
    mapFind(const std::unordered_map<X1, X2>& mapS, const X1& val)
{
    auto map_it = mapS.find(val);
    if (map_it != mapS.end()) {
        return map_it->second;
    }
    return {};
}
