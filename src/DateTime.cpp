#include "DateTime.h"
#include <cstdio>
#include <string>

DateTime::DateTime(const std::string& date,
                   const std::string& time)
{
    int year = str2int(date.substr(0, 4));
    int mon  = str2int(date.substr(5, 2));
    int day  = str2int(date.substr(8, 2));
    int hour = str2int(time.substr(0, 2));
    int min  = str2int(time.substr(3, 2));
    int sec  = str2int(time.substr(6, 2));
    setTime(year, mon, day, hour, min, sec);
}

DateTime::DateTime(int year, int mon, int day, int hour, int min, int sec)
{
    setTime(year, mon, day, hour, min, sec);
}

void DateTime::setTime(int year, int mon, int day, int hour, int min, int sec)
{
    time_.tm_sec   = sec;
    time_.tm_min   = min;
    time_.tm_hour  = hour;
    time_.tm_mday  = day;
    time_.tm_mon   = mon - 1;
    time_.tm_year  = year - 1900;
    time_.tm_isdst = 0;

    secTime_ = mktime(&time_);

    isValid_ = secTime_ != -1 &&
        time_.tm_year == year-1900 &&
        time_.tm_mon == mon-1 &&
        time_.tm_mday == day;
}

std::string DateTime::getDateStr() const
{
    char buf[64];
    sprintf(buf, "%04d-%02d-%02d",
            time_.tm_year+1900,
            time_.tm_mon+1,
            time_.tm_mday);
    return std::string(buf);
}

