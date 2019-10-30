/*
Copyright © 2017-2019,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance
for Sustainable Energy, LLC.  See the top-level NOTICE for additional details.
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause
*/

#include "gmlc/utilities/TimeSeries.hpp"
#include "gtest/gtest.h"
#include <iostream>

using namespace gmlc::utilities;

template class TimeSeries<double, double>;

TEST(timeSeries, basic_test) { 
	TimeSeries<> simple;

	simple.addData(0, 5.0);
    simple.addData(1, 6.0);

	EXPECT_EQ(simple.size(), 2u);
    EXPECT_FALSE(simple.empty());
    EXPECT_EQ(simple.data(0), 5.0);
    EXPECT_EQ(simple.time(0), 0.0);
    EXPECT_EQ(simple.data(1), 6.0);
    EXPECT_EQ(simple.time(1), 1.0);

	simple.clear();

	EXPECT_TRUE(simple.empty());

	
}
