#include "DatePeriod.h"
#include <map>
#include <vector>

#include <iostream>
using namespace std;

DatePeriod::DatePeriod(const std::string& start,
                       const std::string& end,
                       const std::string& period)
    : startDate_(start),
      endDate_(end),
      period_(toByte(period))
{
}

DatePeriod::DatePeriod(const DateTime& start, const DateTime& end, char period)
    : startDate_(start), endDate_(end), period_(period)
{
}

std::string DatePeriod::getPeriod() const
{
    std::string period = "";
    char buf[32];

    for (int i = 1; i < 7; i++)
    {
        char flag = 1 << i;
        if ((flag & period_) != 0)
        {
            sprintf(buf, "%d", i);
            period += buf;
        }
    }
    if ((period_ & 1) != 0)
    {
        period += "7";
    }
    return period;
}

std::string DatePeriod::toString() const
{
    return startDate_.getDateStr() + "|"
        + endDate_.getDateStr() + "|"
        + getPeriod();
}

void DatePeriod::merge(const std::vector<DatePeriod>& in,
                       std::vector<DatePeriod>* out)
{
    std::map<DateTime, char> date;

    for (std::vector<DatePeriod>::const_iterator inIter = in.begin();
         inIter != in.end(); ++inIter)
    {
        date[(*inIter).startDate()] |= 0x1;
        date[(*inIter).endDate()]   |= 0x2;
    }

    std::map<DateTime, char>::iterator dateIter = date.begin();
    std::map<DateTime, char>::iterator dateNextIter = dateIter;
    dateNextIter++;

    while (dateNextIter != date.end())
    {
        DateTime startDate = (*dateIter).first;

        if (((*dateIter).second & 0x1) == 0)
        {
            startDate.addDay(1);
        }

        DateTime endDate = (*dateNextIter).first;

        if (((*dateNextIter).second & 0x2) == 0)
        {
            endDate.addDay(-1);
        }

        char period = 0;
        for (std::vector<DatePeriod>::const_iterator inIter = in.begin();
             inIter != in.end(); ++inIter)
        {
            if ((*inIter).startDate() <= startDate &&
                endDate <= (*inIter).endDate())
            {
                period |= (*inIter).period();
            }
        }

        if (period != 0)
        {
            DatePeriod current(startDate, endDate, period);
            bool needPushed = true;
            if ((*out).size() > 0)
            {
                DatePeriod& last = (*out)[(*out).size()-1];
                if (canBeMerged(last, current))
                {
                    last.setEndDate(current.endDate());
                    last.setPeriod(last.period() | current.period());
                    needPushed = false;
                }
            }
            if (needPushed)
            {
                (*out).push_back(current);
            }
        }
        dateIter++;
        dateNextIter++;
    }
}

bool DatePeriod::canBeMerged(const DatePeriod& first,
                             const DatePeriod& second)
{
    char p1 = first.period();
    char p2 = second.period();
    char same = p1 & p2;
    char p1only = p1 ^ same;
    char p2only = p2 ^ same;
    char pboth = p1 | p2;

    if (hasEffectiveDay(first.startDate(), first.endDate(), p2only) ||
        hasEffectiveDay(second.startDate(), second.endDate(), p1only))
        return false;

    DateTime startDate(first.endDate());
    startDate.addDay(1);
    DateTime endDate(second.startDate());
    endDate.addDay(-1);
    return !hasEffectiveDay(startDate, endDate, pboth);

}

bool DatePeriod::hasEffectiveDay(const DateTime& start, const DateTime& end, char period)
{
    if (period  == 0) return false;

    int dayNum = end.getDiffDayNum(start);
    if (dayNum >= 7) return true;

    int wday = start.getWeekday();
    for (int i = 0; i <= dayNum; i++)
    {
        if (((1 << wday) & period) != 0)
        {
            return true;
        }
        wday = (wday + 1) % 7;
    }

    return false;
}

DatePeriod& DatePeriod::standard()
{
    if (period_  == 0) return *this;

    int dayShift = 0;
    int wday = endDate_.getWeekday();
    while (((1 << wday) & period_) == 0)
    {
        wday = (wday - 1) % 7;
        dayShift--;
    }
    endDate_.addDay(dayShift);

    dayShift = 0;
    wday = startDate_.getWeekday();
    while (((1 << wday) & period_) == 0)
    {
        wday = (wday + 1) % 7;
        dayShift++;
    }
    startDate_.addDay(dayShift);

    int dayNum = endDate_.getDiffDayNum(startDate_);
    if (dayNum <= 7)
    {
        char effective = 0;
        for (int i = 0; i < dayNum; i++)
        {
            effective |= (1 << wday);
            wday = (wday + 1) % 7;
        }
        period_ &= effective;
    }
    return *this;
}

char DatePeriod::toByte(const string& period) const
{
    char p = 0;
    for (size_t i = 0; i < period.size(); ++i)
    {
        p |= (1 << ((period[i]-'0') % 7));
    }
    return p;
}
