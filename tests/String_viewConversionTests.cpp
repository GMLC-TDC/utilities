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

#include "gtest/gtest.h"

#include "gmlc/utilities/string_viewConversion.h"

#include <iostream>

using namespace gmlc::utilities;

TEST(strViewconversion, simple_integer_conversions_test)
{
    auto a = numeric_conversion<int>("457", -1);
    EXPECT_EQ(a, 457);
    auto b = numeric_conversion<long long>("-457", -1);
    EXPECT_EQ(b, -457);
    static_assert(std::is_same<decltype(b), long long>::value,
                  "conversion types do not match");
    auto c = numeric_conversion<unsigned char>("25", 0xFF);
    EXPECT_EQ(c, 25);
    auto d = numeric_conversion<short>("-7629", 0xFF);
    EXPECT_EQ(d, -7629);
    EXPECT_EQ(sizeof(d), 2u);

    auto e = numeric_conversion<unsigned int>("-1", 0);
    EXPECT_EQ(e, static_cast<unsigned int>(-1));

    auto f = numeric_conversion("FF3q", -234);
    EXPECT_EQ(f, -234);
}

TEST(strViewconversion, simple_floating_point_conversions_test)
{
    const double closeDef = 0.0000000001;
    auto a = numeric_conversion<float>("457", -1);
    EXPECT_EQ(a, 457);
    auto b = numeric_conversion<double>("234.123131", -1);
    EXPECT_NEAR(b, 234.123131, closeDef);
    static_assert(std::is_same<decltype(b), double>::value,
                  "conversion types do not match");
    auto c = numeric_conversion<double>(".456", 0xFF);
    EXPECT_NEAR(c, .456, closeDef);
    auto d = numeric_conversion<long double>("45.456e27", 0xFF);
    EXPECT_NEAR(d, 45.456e27, closeDef);
    EXPECT_EQ(sizeof(d), sizeof(long double));

    auto e = numeric_conversion<double>("-456.234", 0);
    EXPECT_NEAR(e, -456.234, closeDef);

    auto f = numeric_conversion<double>("-23E-2", 0);
    EXPECT_NEAR(f, -0.23, closeDef);

    auto g = numeric_conversion("FF3q", 45.34);
    EXPECT_NEAR(g, 45.34, closeDef);
}
