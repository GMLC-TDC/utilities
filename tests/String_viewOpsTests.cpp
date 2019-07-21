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

#include "gmlc/utilities/string_viewOps.h"

#include <iostream>

using namespace gmlc::utilities::string_viewOps;
using gmlc::utilities::string_view;

/** test trim*/
TEST(stringViewOps, trimString_tests)
{
    string_view test1 = "AbCd: *Ty; ";
    test1 = trim(test1);
    EXPECT_TRUE(test1 == "AbCd: *Ty;");
    string_view test2 = "  \t1234:AbC\n\t RTrt\n ";
    test2 = trim(test2);
    EXPECT_TRUE(test2 == "1234:AbC\n\t RTrt");
    // test for an empty results
    string_view test3 = "  \t\n \t\t \n ";
    test3 = trim(test3);
    EXPECT_TRUE(test3.empty());

    // test for other characters
    string_view test4 = "%%**Bill45 *%*%";
    test4 = trim(test4, "*%");
    EXPECT_TRUE(test4 == "Bill45 ");

    string_view test5 = "AbCd: *Ty; ";
    trimString(test5);
    EXPECT_TRUE(test5 == "AbCd: *Ty;");
    string_view test6 = "  \t1234:AbC\n\t RTrt\n ";
    trimString(test6);
    EXPECT_TRUE(test6 == "1234:AbC\n\t RTrt");
    // test for an empty results
    string_view test7 = "  \t\n \t\t \n ";
    trimString(test7);
    EXPECT_TRUE(test7.empty());

    // test for other characters
    string_view test8 = "%%**Bill45 *%*%";
    trimString(test8, "*%");
    EXPECT_TRUE(test8 == "Bill45 ");
}

/** test trim*/
TEST(stringViewOps, tailString_tests)
{
    string_view test1 = "AbCd: *Ty; ";
    auto testres = getTailString(test1, '*');
    EXPECT_TRUE(testres == "Ty; ");
    string_view test2 = "bob::test1:test2:4";
    testres = getTailString(test2, ':');
    EXPECT_TRUE(testres == "4");

    string_view test3 = "::  \t1234:AbC\n\t RTrt\n ::";
    // test with ':' also as a space
    testres = getTailString(test3, ':');
    EXPECT_TRUE(testres.empty());

    string_view test4 = "bob::test1:test2:4";
    testres = getTailString(test4, '-');
    EXPECT_TRUE(testres == test4);

    string_view test5 = "4testingBeginning";
    testres = getTailString(test5, '4');
    EXPECT_TRUE(testres == "testingBeginning");

    string_view test6 = "4testingBeginning";
    testres = getTailString(test6, 'g');
    EXPECT_TRUE(testres.empty());
}

/** simple split line test*/
TEST(stringViewOps, splitline_test1)
{
    string_view test1 = "alpha, bravo, charlie";
    auto testres = split(test1);

    ASSERT_TRUE(testres.size() == 3);
    EXPECT_TRUE(testres[0] == "alpha");
    EXPECT_TRUE(testres[1] == " bravo");
    EXPECT_TRUE(testres[2] == " charlie");

    testres = split(test1, ", ", delimiter_compression::on);

    ASSERT_TRUE(testres.size() == 3);
    EXPECT_TRUE(testres[0] == "alpha");
    EXPECT_TRUE(testres[1] == "bravo");
    EXPECT_TRUE(testres[2] == "charlie");
}

/** simple split line test for multiple tokens*/
TEST(stringViewOps, splitline_test2)
{
    string_view test1 = "alpha, bravo,;, charlie,";
    auto testres = split(test1);

    ASSERT_TRUE(testres.size() == 6);
    EXPECT_TRUE(testres[2].empty());
    EXPECT_TRUE(testres[3].empty());
    EXPECT_TRUE(testres[5].empty());

    testres = split(test1, ";, ", delimiter_compression::on);

    ASSERT_TRUE(testres.size() == 3);
}

/** simple split line and trim test*/
TEST(stringViewOps, splitline_test3)
{
    string_view test1 = " alpha,   bravo ; charlie ";
    auto testres = split(test1);
    trim(testres);
    ASSERT_TRUE(testres.size() == 3);
    EXPECT_TRUE(testres[0] == "alpha");
    EXPECT_TRUE(testres[1] == "bravo");
    EXPECT_TRUE(testres[2] == "charlie");
}

/**remove quotes test*/
TEST(stringViewOps, removeQuotes_test)
{
    string_view test1 = "\'remove quotes\'";
    auto testres = removeQuotes(test1);

    EXPECT_TRUE(testres == "remove quotes");
    string_view test2 = "\"remove quotes \"";
    testres = removeQuotes(test2);

    EXPECT_TRUE(testres == "remove quotes ");

    string_view test3 = "\"remove quotes \'";
    testres = removeQuotes(test3);

    EXPECT_TRUE(testres == "\"remove quotes \'");

    string_view test4 = "   \" remove quotas \"  ";
    testres = removeQuotes(test4);

    EXPECT_TRUE(testres == " remove quotas ");
}

TEST(stringViewOps, splitLineQuotes_tests)
{
    string_view test1 = "454, 345, happy; frog";
    auto testres = splitlineQuotes(test1);
    trim(testres);
    ASSERT_TRUE(testres.size() == 4);
    EXPECT_TRUE(testres[0] == "454");
    EXPECT_TRUE(testres[1] == "345");
    EXPECT_TRUE(testres[2] == "happy");
    EXPECT_TRUE(testres[3] == "frog");
    string_view test2 = " \'alpha,   bravo\' ; charlie ";
    auto testres2 = splitlineQuotes(test2);
    trim(testres2);
    ASSERT_TRUE(testres2.size() == 2);
    EXPECT_TRUE(testres2[0] == "\'alpha,   bravo\'");
    EXPECT_TRUE(testres2[1] == "charlie");

    string_view test3 = " \"test1\",\'test2\' ; \"charlie\",";
    auto testres3 = splitlineQuotes(test3);
    trim(testres3);
    ASSERT_TRUE(testres3.size() == 4);
    EXPECT_TRUE(testres3[0] == "\"test1\"");
    EXPECT_TRUE(testres3[1] == "\'test2\'");
    EXPECT_TRUE(testres3[2] == "\"charlie\"");
    EXPECT_TRUE(testres3[3].empty());

    testres3 = splitlineQuotes(test3, default_delim_chars, default_quote_chars,
                               delimiter_compression::on);
    ASSERT_TRUE(testres3.size() == 3);

    string_view test4 = "\"'part1' and,; 'part2'\",\"34,45,56\"";
    auto testres4 = splitlineQuotes(test4);
    ASSERT_TRUE(testres4.size() == 2);
    EXPECT_TRUE(testres4[1] == "\"34,45,56\"");

    string_view test5 = "\"part1'\" and \"part2\",\"34,45,56\"";
    auto testres5 = splitlineQuotes(test5);
    ASSERT_TRUE(testres5.size() == 2);
    EXPECT_TRUE(testres5[1] == "\"34,45,56\"");
}

TEST(stringViewOps, splitLineBracket_tests)
{
    string_view test1 = "(454, 345), happy; frog";
    auto testres = splitlineBracket(test1);
    trim(testres);
    ASSERT_TRUE(testres.size() == 3);
    EXPECT_TRUE(testres[0] == "(454, 345)");
    EXPECT_TRUE(testres[1] == "happy");
    EXPECT_TRUE(testres[2] == "frog");
    string_view test2 = " \'alpha,   bravo\' ; charlie ";
    // the default bracket split should recognize strings as well
    auto testres2 = splitlineBracket(test2);
    trim(testres2);
    ASSERT_TRUE(testres2.size() == 2);
    EXPECT_TRUE(testres2[0] == "\'alpha,   bravo\'");
    EXPECT_TRUE(testres2[1] == "charlie");

    string_view test3 = "$45,34,45$;$23.45,34,23.3$";
    auto testres3 = splitlineBracket(test3, ";,", "$");
    trim(testres3);
    ASSERT_TRUE(testres3.size() == 2);
    EXPECT_TRUE(testres3[0] == "$45,34,45$");
    EXPECT_TRUE(testres3[1] == "$23.45,34,23.3$");
}