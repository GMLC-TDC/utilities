/*
Copyright (c) 2017-2024,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance
for Sustainable Energy, LLC.  See the top-level NOTICE for additional details.
All rights reserved. SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <optional>

namespace gmlc::utilities {
/** get a specific value from a map like structure
@param mapS the map structure
@param val the val to use as a key
@param defVal the default value to return if val was not found
@return the result value or defVal if the key was not found
*/
template<class MAP, class X1, class X2 = typename MAP::mapped_type>
inline X2 mapFind(const MAP& mapS, const X1& val, const X2& defVal)
{
    auto map_it = mapS.find(val);
    return (map_it != mapS.end()) ? map_it->second : defVal;
}

/** get a specific value from a map like structure
@param mapS the map structure
@param val the val to use as a key
@return on optional which is filled if the key is found
*/
template<class MAP, class X1, class X2 = typename MAP::mapped_type>
std::optional<X2> mapFind(const MAP& mapS, const X1& val)
{
    auto map_it = mapS.find(val);
    if (map_it != mapS.end()) {
        return map_it->second;
    }
    return std::nullopt;
}
}  // namespace gmlc::utilities
