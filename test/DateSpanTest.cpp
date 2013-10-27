#include "gtest/gtest.h"
#include "DateSpan.h"
#include <vector>

using namespace std;

class DateSpanTest: public ::testing::Test
{
};

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

