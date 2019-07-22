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

#include "gtest/gtest.h"

#include "gmlc/utilities/stringOps.h"

#include <iostream>

using namespace gmlc::utilities;
using namespace gmlc::utilities::stringOps;

/** test conversion to lower case*/
TEST(stringops, convert_to_lower_case_test)
{
    std::string test1 = "AbCd: *Ty; ";
    auto testres = convertToLowerCase(test1);
    EXPECT_TRUE(testres == "abcd: *ty; ");
    testres = convertToLowerCase("  1234:ABC\n\t RTrt ");
    EXPECT_TRUE(testres == "  1234:abc\n\t rtrt ");
}

/** test make lower case*/
TEST(stringops, make_lower_case_test)
{
    std::string test1 = "AbCd: *Ty; ";
    makeLowerCase(test1);
    EXPECT_TRUE(test1 == "abcd: *ty; ");
    std::string test2 = "  1234:ABC\n\t RTrt ";
    makeLowerCase(test2);
    EXPECT_TRUE(test2 == "  1234:abc\n\t rtrt ");
}

/** test conversion to upper case*/
TEST(stringops, convert_to_upper_case_test)
{
    std::string test1 = "AbCd: *Ty; ";
    auto testres = convertToUpperCase(test1);
    EXPECT_TRUE(testres == "ABCD: *TY; ");
    testres = convertToUpperCase("  1234:ABC\n\t RTrt ");
    EXPECT_TRUE(testres == "  1234:ABC\n\t RTRT ");
}

/** test make upper case*/
TEST(stringops, make_upper_case_test)
{
    std::string test1 = "AbCd: *Ty; ";
    makeUpperCase(test1);
    EXPECT_TRUE(test1 == "ABCD: *TY; ");
    std::string test2 = "  1234:ABC\n\t RTrt ";
    makeUpperCase(test2);
    EXPECT_TRUE(test2 == "  1234:ABC\n\t RTRT ");
}

/** test trim*/
TEST(stringops, trimString_tests)
{
    std::string test1 = "AbCd: *Ty; ";
    trimString(test1);
    EXPECT_TRUE(test1 == "AbCd: *Ty;");
    std::string test2 = "  \t1234:AbC\n\t RTrt\n ";
    trimString(test2);
    EXPECT_TRUE(test2 == "1234:AbC\n\t RTrt");
    // test for an empty results
    std::string test3 = "  \t\n \t\t \n ";
    trimString(test3);
    EXPECT_TRUE(test3.empty());
}

/** test trim*/
TEST(stringops, trim_tests)
{
    std::string test1 = "AbCd: *Ty; ";
    auto testres = trim(test1);
    EXPECT_TRUE(testres == "AbCd: *Ty;");
    std::string test2 = "  \t1234:AbC\n\t RTrt\n ";
    testres = trim(test2);
    EXPECT_TRUE(testres == "1234:AbC\n\t RTrt");

    std::string test3 = "::  \t1234:AbC\n\t RTrt\n ::";
    // test with ':' also as a space
    testres = trim(test3, "\t\n: ");
    EXPECT_TRUE(testres == "1234:AbC\n\t RTrt");

    std::string test4 = "  \t\n \t\t \n ";
    testres = trim(test4);
    EXPECT_TRUE(testres.empty());
}

/** test trim*/
TEST(stringops, tailString_tests)
{
    std::string test1 = "AbCd: *Ty; ";
    auto testres = getTailString(test1, '*');
    EXPECT_TRUE(testres == "Ty; ");
    std::string test2 = "bob::test1:test2:4";
    testres = getTailString(test2, ':');
    EXPECT_TRUE(testres == "4");

    std::string test3 = "::  \t1234:AbC\n\t RTrt\n ::";
    // test with ':' also as a space
    testres = getTailString(test3, ':');
    EXPECT_TRUE(testres.empty());

    std::string test4 = "bob::test1:test2:4";
    testres = getTailString(test4, '-');
    EXPECT_TRUE(testres == test4);

    std::string test5 = "4testingBeginning";
    testres = getTailString(test5, '4');
    EXPECT_TRUE(testres == "testingBeginning");

    std::string test6 = "4testingBeginning";
    testres = getTailString(test6, 'g');
    EXPECT_TRUE(testres.empty());
}

/** simple split line test*/
TEST(stringops, splitline_test1)
{
    std::string test1 = "alpha, bravo, charlie";
    auto testres = splitline(test1);

    ASSERT_EQ(testres.size(), 3u);
    EXPECT_TRUE(testres[0] == "alpha");
    EXPECT_TRUE(testres[1] == " bravo");
    EXPECT_TRUE(testres[2] == " charlie");

    testres = splitline(test1, ", ", delimiter_compression::on);

    ASSERT_EQ(testres.size(), 3u);
    EXPECT_TRUE(testres[0] == "alpha");
    EXPECT_TRUE(testres[1] == "bravo");
    EXPECT_TRUE(testres[2] == "charlie");
}

/** simple split line test for multiple tokens*/
TEST(stringops, splitline_test2)
{
    std::string test1 = "alpha, bravo,;, charlie,";
    auto testres = splitline(test1);

    ASSERT_EQ(testres.size(), 6u);
    EXPECT_TRUE(testres[2].empty());
    EXPECT_TRUE(testres[3].empty());
    EXPECT_TRUE(testres[5].empty());

    std::string test2 = "alpha, bravo,;, charlie,";
    testres = splitline(test1, ";, ", delimiter_compression::on);

    EXPECT_TRUE(testres.size() == 3);
}

/** simple split line test*/
TEST(stringops, splitline_test3)
{
    std::string test1 = " alpha,   bravo ; charlie ";
    auto testres = splitline(test1);
    trim(testres);
    ASSERT_TRUE(testres.size() == 3);
    EXPECT_TRUE(testres[0] == "alpha");
    EXPECT_TRUE(testres[1] == "bravo");
    EXPECT_TRUE(testres[2] == "charlie");
}

/**remove quotes test test*/
TEST(stringops, removeQuotes_test)
{
    std::string test1 = "\'remove quotes\'";
    auto testres = removeQuotes(test1);

    EXPECT_TRUE(testres == "remove quotes");
    std::string test2 = "\"remove quotes \"";
    testres = removeQuotes(test2);

    EXPECT_TRUE(testres == "remove quotes ");

    std::string test3 = "\"remove quotes \'";
    testres = removeQuotes(test3);

    EXPECT_TRUE(testres == "\"remove quotes \'");

    std::string test4 = "   \" remove quotas \"  ";
    testres = removeQuotes(test4);

    EXPECT_TRUE(testres == " remove quotas ");
}

TEST(stringops, appendInteger_tests)
{
    std::string str1 = "tail_";
    appendInteger(str1, 456u);
    EXPECT_TRUE(str1 == "tail_456");

    std::string str2 = "tail_";
    appendInteger(str2, 0);
    EXPECT_TRUE(str2 == "tail_0");

    std::string str3 = "tail";
    appendInteger(str3, -34);
    EXPECT_TRUE(str3 == "tail-34");

    std::string str4 = "tail";
    appendInteger(str4, 12345678ull);
    EXPECT_TRUE(str4 == "tail12345678");

    // only use the integer part of the number if it is less than 1,000,000,000
    std::string str5 = "num=";
    appendInteger(str5, 98.2341);
    EXPECT_TRUE(str5 == "num=98");

    // the result here is not well defined so this check is mainly to make sure
    // it didn't crash
    std::string str6 = "num=";
    appendInteger(str6, 45e34);
    EXPECT_TRUE(str6 != "num=");

    std::string str7 = "long num_";
    appendInteger(str7, 1234567890123456ll);
    EXPECT_TRUE(str7 == "long num_1234567890123456");

    std::string str8 = "long num_";
    appendInteger(str8, -1234567890123ll);
    EXPECT_TRUE(str8 == "long num_-1234567890123");
}

TEST(stringops, splitLineQuotes_tests)
{
    std::string test1 = "454, 345, happy; frog";
    auto testres = splitlineQuotes(test1);
    trim(testres);
    ASSERT_TRUE(testres.size() == 4);
    EXPECT_TRUE(testres[0] == "454");
    EXPECT_TRUE(testres[1] == "345");
    EXPECT_TRUE(testres[2] == "happy");
    EXPECT_TRUE(testres[3] == "frog");
    std::string test2 = " \'alpha,   bravo\' ; charlie ";
    auto testres2 = splitlineQuotes(test2);
    trim(testres2);
    ASSERT_TRUE(testres2.size() == 2);
    EXPECT_TRUE(testres2[0] == "\'alpha,   bravo\'");
    EXPECT_TRUE(testres2[1] == "charlie");

    std::string test3 = " \"test1\",\'test2\' ; \"charlie\",";
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

    std::string test4 = "\"'part1' and,; 'part2'\",\"34,45,56\"";
    auto testres4 = splitlineQuotes(test4);
    ASSERT_TRUE(testres4.size() == 2);
    EXPECT_TRUE(testres4[1] == "\"34,45,56\"");

    std::string test5 = "\"part1'\" and \"part2\",\"34,45,56\"";
    auto testres5 = splitlineQuotes(test5);
    ASSERT_TRUE(testres5.size() == 2);
    EXPECT_TRUE(testres5[1] == "\"34,45,56\"");

    std::string test6 =
      "--arg1 --arg2=bob --arg3=\"string1 string2\" --arg3=\"bob\"";
    auto testres6 = splitlineQuotes(test6, " \t");
    ASSERT_TRUE(testres6.size() == 4);
    EXPECT_TRUE(testres6[2] == "--arg3=\"string1 string2\"");
}

TEST(stringops, splitLineBracket_tests)
{
    std::string test1 = "(454, 345), happy; frog";
    auto testres = splitlineBracket(test1);
    trim(testres);
    ASSERT_TRUE(testres.size() == 3);
    EXPECT_TRUE(testres[0] == "(454, 345)");
    EXPECT_TRUE(testres[1] == "happy");
    EXPECT_TRUE(testres[2] == "frog");
    std::string test2 = " \'alpha,   bravo\' ; charlie ";
    // the default bracket split should recognize strings as well
    auto testres2 = splitlineBracket(test2);
    trim(testres2);
    ASSERT_TRUE(testres2.size() == 2);
    EXPECT_TRUE(testres2[0] == "\'alpha,   bravo\'");
    EXPECT_TRUE(testres2[1] == "charlie");

    std::string test3 = "$45,34,45$;$23.45,34,23.3$";
    auto testres3 = splitlineBracket(test3, ";,", "$");
    trim(testres3);
    ASSERT_TRUE(testres3.size() == 2);
    EXPECT_TRUE(testres3[0] == "$45,34,45$");
    EXPECT_TRUE(testres3[1] == "$23.45,34,23.3$");
}
