/*
 * LLNS Copyright Start
 * Copyright (c) 2014-2019, Lawrence Livermore National Security
 * This work was performed under the auspices of the U.S. Department
 * of Energy by Lawrence Livermore National Laboratory in part under
 * Contract W-7405-Eng-48 and in part under Contract DE-AC52-07NA27344.
 * Produced at the Lawrence Livermore National Laboratory.
 * All rights reserved.
 * For details, see the LICENSE file.
 * LLNS Copyright End
 */

#include "gmlc/utilities/mapOps.hpp"

#include "gtest/gtest.h"
#include <map>
#include <string>
#include <unordered_map>

using namespace gmlc::utilities;

/** test with a map*/
TEST(mapops, map1)
{
    const std::map<std::string, std::string> map1{
        {"a", "string1"}, {"b", "string2"}};

    EXPECT_EQ(
        mapFind(map1, std::string("a"), std::string("string3")), "string1");
    EXPECT_EQ(
        mapFind(map1, std::string("b"), std::string("string3")), "string2");
    EXPECT_EQ(
        mapFind(map1, std::string("c"), std::string("string3")), "string3");
}

/** test with a map*/
TEST(mapops, map2)
{
    const std::map<int, std::string> map1{{1, "string1"}, {2, "string2"}};

    EXPECT_EQ(mapFind(map1, 1, std::string("string3")), "string1");
    EXPECT_EQ(mapFind(map1, 2, std::string("string3")), "string2");
    EXPECT_EQ(mapFind(map1, 3, std::string("string3")), "string3");
}

/** test with a map*/
TEST(mapops, mapopt1)
{
    const std::map<std::string, std::string> map1{
        {"a", "string1"}, {"b", "string2"}};

    const auto aVal = mapFind(map1, std::string("a"));
    ASSERT_TRUE(aVal.has_value());
    ASSERT_EQ(aVal, std::optional<std::string>{"string1"});

    const auto bVal = mapFind(map1, std::string("b"));
    ASSERT_TRUE(bVal.has_value());
    ASSERT_EQ(bVal, std::optional<std::string>{"string2"});
    EXPECT_FALSE(mapFind(map1, std::string("c")));
}

/** test with a map*/
TEST(mapops, mapopt2)
{
    const std::map<int, std::string> map1{{1, "string1"}, {2, "string2"}};

    const auto oneVal = mapFind(map1, 1);
    ASSERT_TRUE(oneVal.has_value());
    ASSERT_EQ(oneVal, std::optional<std::string>{"string1"});

    const auto twoVal = mapFind(map1, 2);
    ASSERT_TRUE(twoVal.has_value());
    ASSERT_EQ(twoVal, std::optional<std::string>{"string2"});
    EXPECT_FALSE(mapFind(map1, 3));
}

TEST(mapops, umap1)
{
    const std::unordered_map<std::string, std::string> map1{
        {"a", "string1"}, {"b", "string2"}};

    EXPECT_EQ(
        mapFind(map1, std::string("a"), std::string("string3")), "string1");
    EXPECT_EQ(
        mapFind(map1, std::string("b"), std::string("string3")), "string2");
    EXPECT_EQ(
        mapFind(map1, std::string("c"), std::string("string3")), "string3");
}

/** test with a map*/
TEST(mapops, umap2)
{
    const std::unordered_map<int, std::string> map1{{1, "string1"}, {2, "string2"}};

    EXPECT_EQ(mapFind(map1, 1, std::string("string3")), "string1");
    EXPECT_EQ(mapFind(map1, 2, std::string("string3")), "string2");
    EXPECT_EQ(mapFind(map1, 3, std::string("string3")), "string3");
}

/** test with a map*/
TEST(mapops, umapopt1)
{
    const std::unordered_map<std::string, std::string> map1{
        {"a", "string1"}, {"b", "string2"}};

    const auto aVal = mapFind(map1, std::string("a"));
    ASSERT_TRUE(aVal.has_value());
    ASSERT_EQ(aVal, std::optional<std::string>{"string1"});

    const auto bVal = mapFind(map1, std::string("b"));
    ASSERT_TRUE(bVal.has_value());
    ASSERT_EQ(bVal, std::optional<std::string>{"string2"});
    EXPECT_FALSE(mapFind(map1, std::string("c")));
}

/** test with a map*/
TEST(mapops, umapopt2)
{
    const std::unordered_map<int, std::string> map1{{1, "string1"}, {2, "string2"}};

    const auto oneVal = mapFind(map1, 1);
    ASSERT_TRUE(oneVal.has_value());
    ASSERT_EQ(oneVal, std::optional<std::string>{"string1"});

    const auto twoVal = mapFind(map1, 2);
    ASSERT_TRUE(twoVal.has_value());
    ASSERT_EQ(twoVal, std::optional<std::string>{"string2"});
    EXPECT_FALSE(mapFind(map1, 3));
}
