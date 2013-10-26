#ifndef DATE_PERIOD_H_
#define DATE_PERIOD_H_

#include <string>
#include <vector>
#include "DateTime.h"

class DatePeriod
{
public:
    DatePeriod(const std::string& start,
               const std::string& end,
               const std::string& period);
    DatePeriod(const DateTime& start, const DateTime& end, char period);
    ~DatePeriod() { }

    inline bool isValid() const;

    inline void setStartDate(const DateTime& rhs);
    inline const DateTime& startDate() const;
    inline void setEndDate(const DateTime& rhs);
    inline const DateTime& endDate() const;
    inline void setPeriod(char period);
    inline char period() const;
    
    char toByte(const std::string& period) const;

    DatePeriod& standard();
    std::string toString() const;
    std::string getPeriod() const;

    static void merge(const std::vector<DatePeriod>& in,
                      std::vector<DatePeriod>* out);

private:
    static bool canBeMerged(const DatePeriod& first, const DatePeriod& second);
    static bool hasEffectiveDay(const DateTime& start,
                                const DateTime& end,
                                char period);

private:
    DateTime startDate_;
    DateTime endDate_;
    char period_;
};

void DatePeriod::setStartDate(const DateTime& rhs)
{
    startDate_ = rhs;
}

const DateTime&  DatePeriod::startDate() const
{
    return startDate_;
}

void  DatePeriod::setEndDate(const DateTime& rhs)
{
    endDate_ = rhs;
}

const DateTime&  DatePeriod::endDate() const
{
    return endDate_;
}

void DatePeriod::setPeriod(char rhs)
{
    period_ = rhs;
}

char DatePeriod::period() const
{
    return period_;
}

bool DatePeriod::isValid() const
{
    return hasEffectiveDay(startDate_, endDate_, period_);
}

#endif // DATE_PERIOD_H_
