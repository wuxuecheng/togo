#ifndef DATE_SPAN_H_
#define DATE_SPAN_H_

#include <string>
#include <vector>
#include "date_time.h"

class DateSpan
{
public:
    DateSpan(const std::string& from_date, const std::string& to_date, const std::string& period);
    DateSpan(const DateTime& from_date, const DateTime& to_date, char period);
    ~DateSpan();

    inline const DateTime& from_date() const;
    inline void set_from_date(const DateTime& rhs);
    inline void set_from_date(const std::string& rhs);

    inline const DateTime& to_date() const;
    inline void set_to_date(const DateTime& rhs);
    inline void set_to_date(const std::string& rhs);

    inline char period() const;
    inline void set_period(char rhs);
    inline void set_period(const std::string& rhs);

    inline std::string getPeriodStr() const;
    inline std::string toString() const;

    DateSpan& normalize();
    DateSpan& shiftDays(int day_num);
    DateSpan intersect(const DateSpan& rhs) const;
    void substitute(const std::vector<DateSpan>& in, std::vector<DateSpan>* out) const;

    static void merge(const std::vector<DateSpan>& in, std::vector<DateSpan>* out);

protected:
    static std::string periodByteToStr(char period);
    static char periodStrToByte(const std::string& period);
    static bool canBeMerged(const DateSpan& first, const DateSpan& second);
    static bool hasEffectiveDay(const DateTime& start, const DateTime& end, char period);
    static inline bool isWeekdaySet(char period, int day);
    static inline void incWeekday(int* day);

protected:
    // 日期区间为：[from_date_, to_date_)
    // 周期位： 0:SUN, 1:MON, 2:TUS, 3:WED, 4:THU, 5:FRI, 6:SAT
    // 如果 from_date >= to_date_，则没有有效日期
    DateTime from_date_;
    DateTime to_date_;
    char period_;
};

const DateTime& DateSpan::from_date() const
{
    return from_date_;
}

void DateSpan::set_from_date(const DateTime& rhs)
{
    from_date_ = rhs;
}

void DateSpan::set_from_date(const std::string& rhs)
{
    from_date_ = DateTime(rhs);
}

const DateTime& DateSpan::to_date() const
{
    return to_date_;
}

void DateSpan::set_to_date(const DateTime& rhs)
{
    to_date_ = rhs;
}

void DateSpan::set_to_date(const std::string& rhs)
{
    to_date_ = DateTime(rhs);
}

char DateSpan::period() const
{
    return period_;
}

void DateSpan::set_period(char rhs)
{
    period_ = rhs;
}

void DateSpan::set_period(const std::string& rhs)
{
    period_ = periodStrToByte(rhs);
}

std::string DateSpan::getPeriodStr() const
{
    return periodByteToStr(period_);
}

std::string DateSpan::toString() const
{
    return from_date_.getDateStr() + "|" + to_date_.getDateStr() + "|" + getPeriodStr();
}

bool DateSpan::isWeekdaySet(char period, int day)
{
    char flag = 1 << ( day % 7);
    return (flag & period) != 0;
}

void DateSpan::incWeekday(int* day)
{
    *day = *day == 6 ? 0 : *day + 1;
}

#endif // DATE_SPAN_H_

