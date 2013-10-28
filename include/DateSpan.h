#ifndef DATE_SPAN_H_
#define DATE_SPAN_H_

#include <string>
#include <vector>
#include "DateTime.h"

class DateSpan
{
public:
    DateSpan(const std::string& fromDate, const std::string& toDate, const std::string& period);
    DateSpan(const DateTime& fromDate, const DateTime& toDate, char period);
    ~DateSpan();

    inline const DateTime& fromDate() const;
    inline void setFromDate(const DateTime& rhs);
    inline void setFromDate(const std::string& rhs);

    inline const DateTime& toDate() const;
    inline void setToDate(const DateTime& rhs);
    inline void setToDate(const std::string& rhs);

    inline char period() const;
    inline void setPeriod(char rhs);
    inline void setPeriod(const std::string& rhs);

    inline std::string getPeriodStr() const;
    inline std::string toString() const;

    DateSpan& normalize();
    DateSpan& shiftDays(int dayNum);
    DateSpan intersect(const DateSpan& rhs) const;
    void substitute(const std::vector<DateSpan>& in, std::vector<DateSpan>* out) const;

    static void merge(const std::vector<DateSpan>& in, std::vector<DateSpan>* out);

protected:
    static std::string periodByteToStr(char period);
    static char periodStrToByte(const std::string& period);
    static bool canBeMerged(const DateSpan& first, const DateSpan& second);
    static bool hasEffectiveDay(const DateTime& start, const DateTime& end, char period);
    static inline bool isWeekdaySet(char period, int day);

protected:
    DateTime fromDate_;
    DateTime toDate_;
    char period_;
};

const DateTime& DateSpan::fromDate() const
{
    return fromDate_;
}

void DateSpan::setFromDate(const DateTime& rhs)
{
    fromDate_ = rhs;
}

void DateSpan::setFromDate(const std::string& rhs)
{
    fromDate_ = DateTime(rhs);
}

const DateTime& DateSpan::toDate() const
{
    return toDate_;
}

void DateSpan::setToDate(const DateTime& rhs)
{
    toDate_ = rhs;
}

void DateSpan::setToDate(const std::string& rhs)
{
    toDate_ = DateTime(rhs);
}

char DateSpan::period() const
{
    return period_;
}

void DateSpan::setPeriod(char rhs)
{
    period_ = rhs;
}

void DateSpan::setPeriod(const std::string& rhs)
{
    period_ = periodStrToByte(rhs);
}

std::string DateSpan::getPeriodStr() const
{
    return periodByteToStr(period_);
}

std::string DateSpan::toString() const
{
    return fromDate_.getDateStr() + "|" + toDate_.getDateStr() + "|" + getPeriodStr();
}

bool DateSpan::isWeekdaySet(char period, int day)
{
    char flag = 1 << ( day % 7);
    return (flag & period) != 0;
}

#endif // DATE_SPAN_H_

