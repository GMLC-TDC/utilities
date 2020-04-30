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

#include "gmlc/utilities/charMapper.h"

#include "gtest/gtest.h"
#include <iostream>
#include <type_traits>

using namespace gmlc::utilities;
/** test conversion to lower case*/
TEST(charMapper, numericMapper)
{
    auto v = numericMapper();
    auto dtype = std::is_same<decltype(v[1]), bool>::value;
    EXPECT_TRUE(dtype);
    EXPECT_TRUE(v['1']);
    EXPECT_TRUE(v['0']);
    EXPECT_FALSE(v['a']);
    EXPECT_FALSE(v.at('a'));
}

TEST(charMapper, numericStartMapper)
{
    auto v = numericStartMapper();
    auto dtype = std::is_same<decltype(v[1]), bool>::value;
    EXPECT_TRUE(dtype);
}

TEST(charMapper, numericEndMapper)
{
    auto v = numericEndMapper();
    auto dtype = std::is_same<decltype(v[1]), bool>::value;
    EXPECT_TRUE(dtype);
}

TEST(charMapper, base64Mapper)
{
    auto v = base64Mapper();
    auto dtype = std::is_same<decltype(v[1]), unsigned char>::value;
    EXPECT_TRUE(dtype);
}

TEST(charMapper, digitMapper)
{
    auto v = digitMapper();
    auto dtype = std::is_same<decltype(v[1]), unsigned char>::value;
    EXPECT_TRUE(dtype);
}

TEST(charMapper, hexMapper)
{
    auto v = hexMapper();
    auto dtype = std::is_same<decltype(v[1]), unsigned char>::value;
    EXPECT_TRUE(dtype);
}

TEST(charMapper, pairMapper)
{
    auto v = pairMapper();
    auto dtype = std::is_same<decltype(v[1]), unsigned char>::value;
    EXPECT_TRUE(dtype);
}
