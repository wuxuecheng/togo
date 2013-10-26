#include "gtest/gtest.h"
#include "DateTime.h"
#include <string>

using namespace std;

class DateTimeTest: public ::testing::Test
{
};

TEST_F(DateTimeTest, Sunday)
{    
    string str = "2013-10-13";
    DateTime date(str);
    EXPECT_TRUE(date.isValid());
    EXPECT_EQ(str, date.getDateStr());
    EXPECT_EQ(7, date.getWeekday()) << "date: " << str;
}

TEST_F(DateTimeTest, Monday)
{    
    string str = "2013-10-14";
    DateTime date(str);
    EXPECT_TRUE(date.isValid());
    EXPECT_EQ(str, date.getDateStr());
    EXPECT_EQ(1, date.getWeekday()) << "date: " << str;
}

TEST_F(DateTimeTest, Tuesday)
{    
    string str = "2013-10-15";
    DateTime date(str);
    EXPECT_TRUE(date.isValid());
    EXPECT_EQ(str, date.getDateStr());
    EXPECT_EQ(2, date.getWeekday()) << "date: " << str;
}

TEST_F(DateTimeTest, Wednesday)
{    
    string str = "2013-10-16";
    DateTime date(str);
    EXPECT_TRUE(date.isValid());
    EXPECT_EQ(str, date.getDateStr());
    EXPECT_EQ(3, date.getWeekday()) << "date: " << str;
}

TEST_F(DateTimeTest, Thursday)
{    
    string str = "2013-10-17";
    DateTime date(str);
    EXPECT_TRUE(date.isValid());
    EXPECT_EQ(str, date.getDateStr());
    EXPECT_EQ(4, date.getWeekday()) << "date: " << str;
}

TEST_F(DateTimeTest, Friday)
{    
    string str = "2013-10-18";
    DateTime date(str);
    EXPECT_TRUE(date.isValid());
    EXPECT_EQ(str, date.getDateStr());
    EXPECT_EQ(5, date.getWeekday()) << "date: " << str;
}

TEST_F(DateTimeTest, Saturday)
{    
    string str = "2013-10-19";
    DateTime date(str);
    EXPECT_TRUE(date.isValid());
    EXPECT_EQ(str, date.getDateStr());
    EXPECT_EQ(6, date.getWeekday()) << "date: " << str;
}

