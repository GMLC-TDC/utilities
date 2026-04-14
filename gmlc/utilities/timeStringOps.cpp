/*
Copyright (c) 2017-2026,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance
for Sustainable Energy, LLC.  See the top-level NOTICE for additional details.
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/
#include "timeStringOps.hpp"

#include "stringOps.h"
#include "string_viewConversion.h"
#include "string_viewOps.h"
#include "timeRepresentation.hpp"

#include <cstddef>
#include <map>
#include <stdexcept>
#include <string>
#include <string_view>

namespace gmlc::utilities {
namespace {
    const std::map<std::string_view, time_units>& timeUnitStrings()
    {
        static const std::map<std::string_view, time_units> time_unitstrings{
            {"ps", time_units::ps},
            {"ns", time_units::ns},
            {"us", time_units::us},
            {"ms", time_units::ms},
            {"s", time_units::s},
            {"sec", time_units::sec},
            // don't want empty string to error default is sec
            {"", time_units::sec},
            {"seconds", time_units::sec},
            {"second", time_units::sec},
            {"min", time_units::minutes},
            {"minute", time_units::minutes},
            {"minutes", time_units::minutes},
            {"hr", time_units::hr},
            {"hour", time_units::hr},
            {"hours", time_units::hr},
            {"day", time_units::day},
            {"week", time_units::week},
            {"wk", time_units::week}};
        return time_unitstrings;
    }
}  // namespace

time_units timeUnitsFromString(std::string_view unitString)
{
    auto fnd = timeUnitStrings().find(unitString);
    if (fnd != timeUnitStrings().end()) {
        return fnd->second;
    }
    const std::string lcUstring =
        convertToLowerCase(string_viewOps::trim(unitString));
    fnd = timeUnitStrings().find(lcUstring);
    if (fnd != timeUnitStrings().end()) {
        return fnd->second;
    }
    throw(std::invalid_argument(
        std::string("unit ") + std::string(unitString) + " not recognized"));
}

double getTimeValue(std::string_view timeString, time_units defUnit)
{
    size_t pos;
    const double val = numConvComp<double>(timeString, pos);
    if (pos >= timeString.size()) {
        return val * toSecondMultiplier(defUnit);
    }
    auto units = string_viewOps::trim(timeString.substr(pos));
    return val * toSecondMultiplier(timeUnitsFromString(units));
}

}  // namespace gmlc::utilities
