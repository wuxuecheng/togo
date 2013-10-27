#ifndef DATE_TIME_H_
#define DATE_TIME_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

class DateTime
{
public:
    DateTime(const std::string& date, const std::string& time = "00:00:00");
    DateTime(int year, int mon, int day, int hour, int min, int sec);
    virtual ~DateTime() { }

    inline bool operator<(const DateTime& rhs) const;
    inline bool operator<=(const DateTime& rhs) const;
    inline bool isValid() const;

    inline int getWeekday() const;
    inline int getDiffDayNum(const DateTime& rhs) const;
    inline DateTime& addDay(int n);
    std::string getDateStr() const;

protected:
    inline int str2int(const std::string& str) const;
    void setTime(int year, int mon, int day, int hour, int min, int sec);

protected:
    bool isValid_;
    time_t secTime_;
    struct tm time_;
};

bool DateTime::isValid() const
{
    return isValid_;
}

DateTime& DateTime::addDay(int n)
{
    secTime_ += n*24*60*60;
    struct tm* newTime = localtime(&secTime_);
    memcpy(&time_, newTime, sizeof(time_));
    return *this;
}

int DateTime::getWeekday() const
{
    return time_.tm_wday == 0 ? 7 : time_.tm_wday;
}

int DateTime::getDiffDayNum(const DateTime& rhs) const
{
    return secTime_/(24*60*60) - rhs.secTime_/(24*60*60);
}

int DateTime::str2int(const std::string& str) const
{
    return atoi(str.c_str());
}

bool DateTime::operator<(const DateTime& rhs) const
{
    return secTime_ < rhs.secTime_;
}

bool DateTime::operator<=(const DateTime& rhs) const
{
    return secTime_ <= rhs.secTime_;
}

#endif // DATE_TIME_H_

