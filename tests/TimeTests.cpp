/*
Copyright (c) 2017-2019,
Battelle Memorial Institute; Lawrence Livermore National Security, LLC; Alliance
for Sustainable Energy, LLC.  See the top-level NOTICE for additional details.
All rights reserved. SPDX-License-Identifier: BSD-3-Clause
*/
#include <chrono>
#include <gtest/gtest.h>

/** these test cases test data_block and data_view objects
 */

#include "gmlc/utilities/timeRepresentation.hpp"

using Time9 = TimeRepresentation<count_time<9>>;
using Time6 = TimeRepresentation<count_time<6>>;
using Time12 = TimeRepresentation<count_time<12>>;

TEST(time9, simple_times)
{
    Time9 time1(10.0);
    EXPECT_EQ(static_cast<double>(time1), 10.0);

    Time9 time2(5, time_units::sec);
    Time9 time3(5000, time_units::ms);
    EXPECT_EQ(time2, time3);

    EXPECT_EQ(time2.toCount(time_units::s), 5);
    EXPECT_EQ(time2.toCount(time_units::ms), 5000);
    EXPECT_EQ(time2.toCount(time_units::us), 5'000'000);
    EXPECT_EQ(time2.toCount(time_units::ns), 5'000'000'000);
    EXPECT_EQ(time2.toCount(time_units::ps), 5'000'000'000'000);
    EXPECT_EQ(time2.toCount(time_units::minutes), 0);

    time3 = 5.01;
    EXPECT_NE(time2, time3);

    EXPECT_EQ(time3.toCount(time_units::s), 5);
    EXPECT_EQ(time3.toCount(time_units::ms), 5010);
    EXPECT_EQ(time3.toCount(time_units::us), 5'010'000);
    EXPECT_EQ(time3.toCount(time_units::ns), 5'010'000'000);
    EXPECT_EQ(time3.toCount(time_units::ps), 5'010'000'000'000);
    EXPECT_EQ(time3.toCount(time_units::minutes), 0);

    time3 = 60.1;
    EXPECT_EQ(time3.toCount(time_units::s), 60);
    EXPECT_EQ(time3.toCount(time_units::ms), 60100);
    EXPECT_EQ(time3.toCount(time_units::minutes), 1);
}

TEST(time9, baseConversion)
{
    Time9 time1(49.759632);

    auto cnt = time1.getBaseTimeCode();
    Time9 time2;
    time2.setBaseTimeCode(cnt);

    EXPECT_EQ(time1, time2);

    Time9 time3(-3562.28963);

    cnt = time3.getBaseTimeCode();
    Time9 time4;
    time4.setBaseTimeCode(cnt);

    EXPECT_EQ(time3, time4);
}

TEST(time9, math)
{
    Time9 time1(4.3);
    Time9 time2(2.7);

    EXPECT_EQ(time1 + time2, Time9(7.0));
    EXPECT_EQ(time1 + 1.7, Time9(6.0));

    EXPECT_EQ(time1 - time2, Time9(1.6));

    EXPECT_EQ(-time1, Time9(-4.3));

    EXPECT_EQ(Time9(2.0) * 5, Time9(10.0));
    EXPECT_EQ(Time9(10.0) / 4, Time9(2.5));
    EXPECT_EQ(Time9(10.0) / 2.5, Time9(4.0));
    EXPECT_EQ(4 * Time9(2.0), Time9(8.0));
    EXPECT_EQ(2.5 * Time9(2.0), Time9(5.0));

    time1 += time2;
    EXPECT_EQ(time1, Time9(7.0));
    time1 -= time2;
    EXPECT_EQ(time1, Time9(4.3));

    auto time3 = Time9(1.0);
    time3 *= 4;
    EXPECT_EQ(time3, Time9(4.0));
    time3 *= 2.5;
    EXPECT_EQ(time3, Time9(10.0));
    time3 /= 2;
    EXPECT_EQ(time3, Time9(5.0));
    time3 /= 2.5;
    EXPECT_EQ(time3, Time9(2.0));
}

TEST(time9, rounding)
{
    EXPECT_TRUE(Time9(1.25e-9) == Time9(1, time_units::ns));
    EXPECT_TRUE(Time9(0.99e-9) == Time9(1, time_units::ns));
    EXPECT_TRUE(Time9(1.49e-9) == Time9(1, time_units::ns));
    EXPECT_TRUE(Time9(1.51e-9) == Time9(2, time_units::ns));
}

TEST(time9, comparison)
{
    EXPECT_TRUE(Time9(1.1) > Time9(1.0));
    EXPECT_TRUE(Time9(-1.1) < Time9(-1.0));
    EXPECT_TRUE(Time9(1.0) < Time9(1.1));
    EXPECT_TRUE(Time9(-1.0) > Time9(-1.1));
    EXPECT_TRUE(Time9(10, time_units::ms) == Time9(10000, time_units::us));

    EXPECT_TRUE(Time9(1.1) >= Time9(1.0));
    EXPECT_TRUE(Time9(-1.1) <= Time9(-1.0));
    EXPECT_TRUE(Time9(1.0) <= Time9(1.1));
    EXPECT_TRUE(Time9(-1.0) >= Time9(-1.1));

    EXPECT_TRUE(Time9(1.0) >= Time9(1.0));
    EXPECT_TRUE(Time9(-1.0) <= Time9(-1.0));

    EXPECT_TRUE(Time9(1.0) != Time9(1.00001));
    EXPECT_TRUE(!(Time9(-1.0) != Time9(-1.0)));

    // now with doubles as the second operand
    EXPECT_TRUE(Time9(1.1) > 1.0);
    EXPECT_TRUE(Time9(-1.1) < -1.0);
    EXPECT_TRUE(Time9(1.0) < 1.1);
    EXPECT_TRUE(Time9(-1.0) > -1.1);
    EXPECT_TRUE(Time9(10, time_units::ms) == 0.01);

    EXPECT_TRUE(Time9(1.1) >= 1.0);
    EXPECT_TRUE(Time9(-1.1) <= -1.0);
    EXPECT_TRUE(Time9(1.0) <= 1.1);
    EXPECT_TRUE(Time9(-1.0) >= -1.1);

    EXPECT_TRUE(Time9(1.0) >= 1.0);
    EXPECT_TRUE(Time9(-1.0) <= -1.0);

    EXPECT_TRUE(Time9(1.0) != 1.00001);
    EXPECT_TRUE(!(Time9(-1.0) != -1.0));

    // now with doubles as the first operand
    EXPECT_TRUE(1.1 > Time9(1.0));
    EXPECT_TRUE(-1.1 < Time9(-1.0));
    EXPECT_TRUE(1.0 < Time9(1.1));
    EXPECT_TRUE(-1.0 > Time9(-1.1));
    EXPECT_TRUE(0.01 == Time9(10000, time_units::us));

    EXPECT_TRUE(1.1 >= Time9(1.0));
    EXPECT_TRUE(-1.1 <= Time9(-1.0));
    EXPECT_TRUE(1.0 <= Time9(1.1));
    EXPECT_TRUE(-1.0 >= Time9(-1.1));

    EXPECT_TRUE(1.0 >= Time9(1.0));
    EXPECT_TRUE(-1.0 <= Time9(-1.0));

    EXPECT_TRUE(1.0 != Time9(1.00001));
    EXPECT_TRUE(!(-1.0 != Time9(-1.0)));
}

TEST(time9, chrono)
{
    using namespace std::chrono;
    milliseconds tm1(100);

    Time9 b(tm1);
    EXPECT_EQ(b, 0.1);

    nanoseconds tmns(10026523523);
    Time9 b2(tmns);
    EXPECT_EQ(b2.getBaseTimeCode(), tmns.count());

    EXPECT_TRUE(b2.to_ns() == tmns);
}

// Time 6 tests
TEST(time6, simple_times)
{
    Time6 time1(10.0);
    EXPECT_EQ(static_cast<double>(time1), 10.0);

    Time6 time2(5, time_units::sec);
    Time6 time3(5000, time_units::ms);
    EXPECT_EQ(time2, time3);

    EXPECT_EQ(time2.toCount(time_units::s), 5);
    EXPECT_EQ(time2.toCount(time_units::ms), 5000);
    EXPECT_EQ(time2.toCount(time_units::us), 5'000'000);
    EXPECT_EQ(time2.toCount(time_units::ns), 5'000'000'000);
    EXPECT_EQ(time2.toCount(time_units::ps), 5'000'000'000'000);
    EXPECT_EQ(time2.toCount(time_units::minutes), 0);

    time3 = 5.01;
    EXPECT_NE(time2, time3);

    EXPECT_EQ(time3.toCount(time_units::s), 5);
    EXPECT_EQ(time3.toCount(time_units::ms), 5010);
    EXPECT_EQ(time3.toCount(time_units::us), 5'010'000);
    EXPECT_EQ(time3.toCount(time_units::ns), 5'010'000'000);
    EXPECT_EQ(time3.toCount(time_units::ps), 5'010'000'000'000);
    EXPECT_EQ(time3.toCount(time_units::minutes), 0);

    time3 = 60.1;
    EXPECT_EQ(time3.toCount(time_units::s), 60);
    EXPECT_EQ(time3.toCount(time_units::ms), 60100);
    EXPECT_EQ(time3.toCount(time_units::minutes), 1);
}

TEST(time6, baseConversion)
{
    Time6 time1(49.759632);

    auto cnt = time1.getBaseTimeCode();
    Time6 time2;
    time2.setBaseTimeCode(cnt);

    EXPECT_EQ(time1, time2);

    Time6 time3(-3562.28963);

    cnt = time3.getBaseTimeCode();
    Time6 time4;
    time4.setBaseTimeCode(cnt);

    EXPECT_EQ(time3, time4);
}

TEST(time6, math)
{
    Time6 time1(4.3);
    Time6 time2(2.7);

    EXPECT_EQ(time1 + time2, Time6(7.0));
    EXPECT_EQ(time1 + 1.7, Time6(6.0));

    EXPECT_EQ(time1 - time2, Time6(1.6));

    EXPECT_EQ(-time1, Time6(-4.3));

    EXPECT_EQ(Time6(2.0) * 5, Time6(10.0));
    EXPECT_EQ(Time6(10.0) / 4, Time6(2.5));
    EXPECT_EQ(Time6(10.0) / 2.5, Time6(4.0));
    EXPECT_EQ(4 * Time6(2.0), Time6(8.0));
    EXPECT_EQ(2.5 * Time6(2.0), Time6(5.0));

    time1 += time2;
    EXPECT_EQ(time1, Time6(7.0));
    time1 -= time2;
    EXPECT_EQ(time1, Time6(4.3));

    auto time3 = Time6(1.0);
    time3 *= 4;
    EXPECT_EQ(time3, Time6(4.0));
    time3 *= 2.5;
    EXPECT_EQ(time3, Time6(10.0));
    time3 /= 2;
    EXPECT_EQ(time3, Time6(5.0));
    time3 /= 2.5;
    EXPECT_EQ(time3, Time6(2.0));
}

TEST(time6, rounding)
{
    EXPECT_TRUE(Time6(1.25e-9) == Time6(1, time_units::ns));
    EXPECT_TRUE(Time6(0.99e-9) == Time6(1, time_units::ns));
    EXPECT_TRUE(Time6(1.49e-9) == Time6(1, time_units::ns));
    EXPECT_TRUE(Time6(1.51e-9) == Time6(2, time_units::ns));
}

TEST(time6, comparison)
{
    EXPECT_TRUE(Time6(1.1) > Time6(1.0));
    EXPECT_TRUE(Time6(-1.1) < Time6(-1.0));
    EXPECT_TRUE(Time6(1.0) < Time6(1.1));
    EXPECT_TRUE(Time6(-1.0) > Time6(-1.1));
    EXPECT_TRUE(Time6(10, time_units::ms) == Time6(10000, time_units::us));

    EXPECT_TRUE(Time6(1.1) >= Time6(1.0));
    EXPECT_TRUE(Time6(-1.1) <= Time6(-1.0));
    EXPECT_TRUE(Time6(1.0) <= Time6(1.1));
    EXPECT_TRUE(Time6(-1.0) >= Time6(-1.1));

    EXPECT_TRUE(Time6(1.0) >= Time6(1.0));
    EXPECT_TRUE(Time6(-1.0) <= Time6(-1.0));

    EXPECT_TRUE(Time6(1.0) != Time6(1.00001));
    EXPECT_TRUE(!(Time6(-1.0) != Time6(-1.0)));

    // now with doubles as the second operand
    EXPECT_TRUE(Time6(1.1) > 1.0);
    EXPECT_TRUE(Time6(-1.1) < -1.0);
    EXPECT_TRUE(Time6(1.0) < 1.1);
    EXPECT_TRUE(Time6(-1.0) > -1.1);
    EXPECT_TRUE(Time6(10, time_units::ms) == 0.01);

    EXPECT_TRUE(Time6(1.1) >= 1.0);
    EXPECT_TRUE(Time6(-1.1) <= -1.0);
    EXPECT_TRUE(Time6(1.0) <= 1.1);
    EXPECT_TRUE(Time6(-1.0) >= -1.1);

    EXPECT_TRUE(Time6(1.0) >= 1.0);
    EXPECT_TRUE(Time6(-1.0) <= -1.0);

    EXPECT_TRUE(Time6(1.0) != 1.00001);
    EXPECT_TRUE(!(Time6(-1.0) != -1.0));

    // now with doubles as the first operand
    EXPECT_TRUE(1.1 > Time6(1.0));
    EXPECT_TRUE(-1.1 < Time6(-1.0));
    EXPECT_TRUE(1.0 < Time6(1.1));
    EXPECT_TRUE(-1.0 > Time6(-1.1));
    EXPECT_TRUE(0.01 == Time6(10000, time_units::us));

    EXPECT_TRUE(1.1 >= Time6(1.0));
    EXPECT_TRUE(-1.1 <= Time6(-1.0));
    EXPECT_TRUE(1.0 <= Time6(1.1));
    EXPECT_TRUE(-1.0 >= Time6(-1.1));

    EXPECT_TRUE(1.0 >= Time6(1.0));
    EXPECT_TRUE(-1.0 <= Time6(-1.0));

    EXPECT_TRUE(1.0 != Time6(1.00001));
    EXPECT_TRUE(!(-1.0 != Time6(-1.0)));
}

TEST(time6, chrono)
{
    using namespace std::chrono;
    milliseconds tm1(100);

    Time6 b(tm1);
    EXPECT_EQ(b, 0.1);

    microseconds tmns(10026523);
    Time6 b2(tmns);
    EXPECT_EQ(b2.getBaseTimeCode(), tmns.count());

    EXPECT_TRUE(std::chrono::duration_cast<microseconds>(b2.to_ns()) == tmns);
}

// Time 12 tests
TEST(time12, simple_times)
{
    Time12 time1(10.0);
    EXPECT_EQ(static_cast<double>(time1), 10.0);

    Time12 time2(5, time_units::sec);
    Time12 time3(5000, time_units::ms);
    EXPECT_EQ(time2, time3);

    EXPECT_EQ(time2.toCount(time_units::s), 5);
    EXPECT_EQ(time2.toCount(time_units::ms), 5000);
    EXPECT_EQ(time2.toCount(time_units::us), 5'000'000);
    EXPECT_EQ(time2.toCount(time_units::ns), 5'000'000'000);
    EXPECT_EQ(time2.toCount(time_units::ps), 5'000'000'000'000);
    EXPECT_EQ(time2.toCount(time_units::minutes), 0);

    time3 = 5.01;
    EXPECT_NE(time2, time3);

    EXPECT_EQ(time3.toCount(time_units::s), 5);
    EXPECT_EQ(time3.toCount(time_units::ms), 5010);
    EXPECT_EQ(time3.toCount(time_units::us), 5'010'000);
    EXPECT_EQ(time3.toCount(time_units::ns), 5'010'000'000);
    EXPECT_EQ(time3.toCount(time_units::ps), 5'010'000'000'000);
    EXPECT_EQ(time3.toCount(time_units::minutes), 0);

    time3 = 60.1;
    EXPECT_EQ(time3.toCount(time_units::s), 60);
    EXPECT_EQ(time3.toCount(time_units::ms), 60100);
    EXPECT_EQ(time3.toCount(time_units::minutes), 1);
}

TEST(time12, baseConversion)
{
    Time12 time1(49.759632);

    auto cnt = time1.getBaseTimeCode();
    Time12 time2;
    time2.setBaseTimeCode(cnt);

    EXPECT_EQ(time1, time2);

    Time12 time3(-3562.28963);

    cnt = time3.getBaseTimeCode();
    Time12 time4;
    time4.setBaseTimeCode(cnt);

    EXPECT_EQ(time3, time4);
}

TEST(time12, math)
{
    Time12 time1(4.3);
    Time12 time2(2.7);

    EXPECT_EQ(time1 + time2, Time12(7.0));
    EXPECT_EQ(time1 + 1.7, Time12(6.0));

    EXPECT_EQ(time1 - time2, Time12(1.6));

    EXPECT_EQ(-time1, Time12(-4.3));

    EXPECT_EQ(Time12(2.0) * 5, Time12(10.0));
    EXPECT_EQ(Time12(10.0) / 4, Time12(2.5));
    EXPECT_EQ(Time12(10.0) / 2.5, Time12(4.0));
    EXPECT_EQ(4 * Time12(2.0), Time12(8.0));
    EXPECT_EQ(2.5 * Time12(2.0), Time12(5.0));

    time1 += time2;
    EXPECT_EQ(time1, Time12(7.0));
    time1 -= time2;
    EXPECT_EQ(time1, Time12(4.3));

    auto time3 = Time12(1.0);
    time3 *= 4;
    EXPECT_EQ(time3, Time12(4.0));
    time3 *= 2.5;
    EXPECT_EQ(time3, Time12(10.0));
    time3 /= 2;
    EXPECT_EQ(time3, Time12(5.0));
    time3 /= 2.5;
    EXPECT_EQ(time3, Time12(2.0));
}

TEST(time12, rounding)
{
    EXPECT_TRUE(Time12(1.25e-12) == Time12(1, time_units::ps));
    EXPECT_TRUE(Time12(0.99e-12) == Time12(1, time_units::ps));
    EXPECT_TRUE(Time12(1.49e-12) == Time12(1, time_units::ps));
    EXPECT_TRUE(Time12(1.51e-12) == Time12(2, time_units::ps));
}

TEST(time12, comparison)
{
    EXPECT_TRUE(Time12(1.1) > Time12(1.0));
    EXPECT_TRUE(Time12(-1.1) < Time12(-1.0));
    EXPECT_TRUE(Time12(1.0) < Time12(1.1));
    EXPECT_TRUE(Time12(-1.0) > Time12(-1.1));
    EXPECT_TRUE(Time12(10, time_units::ms) == Time12(10000, time_units::us));

    EXPECT_TRUE(Time12(1.1) >= Time12(1.0));
    EXPECT_TRUE(Time12(-1.1) <= Time12(-1.0));
    EXPECT_TRUE(Time12(1.0) <= Time12(1.1));
    EXPECT_TRUE(Time12(-1.0) >= Time12(-1.1));

    EXPECT_TRUE(Time12(1.0) >= Time12(1.0));
    EXPECT_TRUE(Time12(-1.0) <= Time12(-1.0));

    EXPECT_TRUE(Time12(1.0) != Time12(1.00001));
    EXPECT_TRUE(!(Time12(-1.0) != Time12(-1.0)));

    // now with doubles as the second operand
    EXPECT_TRUE(Time12(1.1) > 1.0);
    EXPECT_TRUE(Time12(-1.1) < -1.0);
    EXPECT_TRUE(Time12(1.0) < 1.1);
    EXPECT_TRUE(Time12(-1.0) > -1.1);
    EXPECT_TRUE(Time12(10, time_units::ms) == 0.01);

    EXPECT_TRUE(Time12(1.1) >= 1.0);
    EXPECT_TRUE(Time12(-1.1) <= -1.0);
    EXPECT_TRUE(Time12(1.0) <= 1.1);
    EXPECT_TRUE(Time12(-1.0) >= -1.1);

    EXPECT_TRUE(Time12(1.0) >= 1.0);
    EXPECT_TRUE(Time12(-1.0) <= -1.0);

    EXPECT_TRUE(Time12(1.0) != 1.00001);
    EXPECT_TRUE(!(Time12(-1.0) != -1.0));

    // now with doubles as the first operand
    EXPECT_TRUE(1.1 > Time12(1.0));
    EXPECT_TRUE(-1.1 < Time12(-1.0));
    EXPECT_TRUE(1.0 < Time12(1.1));
    EXPECT_TRUE(-1.0 > Time12(-1.1));
    EXPECT_TRUE(0.01 == Time12(10000, time_units::us));

    EXPECT_TRUE(1.1 >= Time12(1.0));
    EXPECT_TRUE(-1.1 <= Time12(-1.0));
    EXPECT_TRUE(1.0 <= Time12(1.1));
    EXPECT_TRUE(-1.0 >= Time12(-1.1));

    EXPECT_TRUE(1.0 >= Time12(1.0));
    EXPECT_TRUE(-1.0 <= Time12(-1.0));

    EXPECT_TRUE(1.0 != Time12(1.00001));
    EXPECT_TRUE(!(-1.0 != Time12(-1.0)));
}

TEST(time12, chrono)
{
    using namespace std::chrono;
    milliseconds tm1(100);

    Time12 b(tm1);
    EXPECT_EQ(b, 0.1);

    nanoseconds tmns(10026523523);
    Time12 b2(tmns);
    EXPECT_EQ(b2.getBaseTimeCode(), tmns.count() * 1000);

    EXPECT_TRUE(b2.to_ns() == tmns);
}
