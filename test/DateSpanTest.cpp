#include "gtest/gtest.h"
#include "DateSpan.h"
#include <vector>

using namespace std;

class DateSpanTest: public ::testing::Test
{
};

TEST_F(DateSpanTest, normalize)
{
    DateSpan dp("2013-10-14", "2013-10-18", "237");
    dp.normalize();
    EXPECT_EQ("2013-10-15|2013-10-16|23", dp.toString());

    dp.setPeriod("2");
    dp.normalize();
    EXPECT_EQ("2013-10-15|2013-10-15|2", dp.toString());

    dp.setPeriod("34");
    dp.normalize();
    EXPECT_EQ("0000-00-00|0000-00-00|", dp.toString());
}

TEST_F(DateSpanTest, shiftDays)
{
    DateSpan dp("2013-10-13", "2013-10-16", "1237");
    dp.shiftDays(2);
    EXPECT_EQ("2013-10-15|2013-10-18|2345", dp.toString());
    dp.shiftDays(-2);
    EXPECT_EQ("2013-10-13|2013-10-16|1237", dp.toString());
}

TEST_F(DateSpanTest, SamePeriod)
{
    DateSpan dp1("2013-10-13", "2013-10-16", "1237");
    DateSpan dp2("2013-10-20", "2013-10-26", "1237");

    vector<DateSpan> in;
    vector<DateSpan> out;

    in.push_back(dp1);
    in.push_back(dp2);

    DateSpan::merge(in, &out);
    ASSERT_EQ((size_t)1, out.size());
    EXPECT_EQ("2013-10-13|2013-10-26|1237", out[0].toString());
}

