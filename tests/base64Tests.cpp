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
