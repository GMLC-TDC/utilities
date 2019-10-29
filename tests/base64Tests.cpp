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

#include "gmlc/utilities/base64.h"
#include "gtest/gtest.h"

using namespace gmlc::utilities;
/** test conversion to lower case*/
TEST(base64, encode)
{
    const char encodeSeq[] = "test sequence";
    auto encoded = base64_encode(encodeSeq, sizeof(encodeSeq));
    auto decoded = base64_decode_to_string(encoded);

    EXPECT_STREQ(encodeSeq, decoded.c_str());
}

TEST(base64, decode_vector)
{
    const char encodeSeq[] = "test sequence";
    auto encoded = base64_encode(encodeSeq, sizeof(encodeSeq) - 1);
    auto decoded = base64_decode(encoded);

    EXPECT_EQ(strlen(encodeSeq), decoded.size());

    std::string res(decoded.begin(), decoded.end());
    EXPECT_EQ(res, encodeSeq);
}

TEST(base64, decode_raw)
{
    const char encodeSeq[] = "test sequence";
    auto encoded = base64_encode(encodeSeq, sizeof(encodeSeq) - 1);
    char result[100];
    auto size = base64_decode(encoded, result, 100);

    EXPECT_EQ(strlen(encodeSeq), size);
    std::string res(result, size);
    EXPECT_EQ(res, encodeSeq);

    for (size_t jj = 11; jj >= 1; jj--)
    {
        memset(result, 0, 100);
        size = base64_decode(encoded, result, jj);
        EXPECT_EQ(size, jj);
        std::string sub = res.substr(0, jj);
        std::string ressub(result, size);
        EXPECT_EQ(ressub, sub);
    }
}
