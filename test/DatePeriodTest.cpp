#include "gtest/gtest.h"
#include "DatePeriod.h"
#include <vector>

using namespace std;

class DatePeriodTest: public ::testing::Test
{
};

TEST_F(DatePeriodTest, SamePeriod)
{
    DatePeriod dp1("2013-10-13", "2013-10-16", "1237");
    DatePeriod dp2("2013-10-20", "2013-10-26", "1237");

    vector<DatePeriod> in;
    vector<DatePeriod> out;

    in.push_back(dp1);
    in.push_back(dp2);

    DatePeriod::merge(in, &out);
    ASSERT_EQ((size_t)1, out.size());
    EXPECT_EQ("2013-10-13|2013-10-26|1237", out[0].toString());
}

