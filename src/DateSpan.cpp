#include "DateSpan.h"
#include <map>
#include <vector>


DateSpan::DateSpan(const std::string& fromDate,
                   const std::string& toDate,
                   const std::string& period)
    : fromDate_(fromDate),
      toDate_(toDate)
{
    period_ = periodStrToByte(period);
}

DateSpan::DateSpan(const DateTime& fromDate,
                   const DateTime& toDate,
                   char period)
    : fromDate_(fromDate), toDate_(toDate), period_(period)
{
}

DateSpan::~DateSpan()
{
}

std::string DateSpan::periodByteToStr(char period)
{
    std::string strPeriod = "";
    char buf[4];

    int wday = 1;
    while (wday < 8)
    {
        if (isWeekdaySet(period, wday))
        {
            sprintf(buf, "%d", wday);
            strPeriod += buf;
        }
        wday++;
    }

    return strPeriod;
}

char DateSpan::periodStrToByte(const std::string& period)
{
    char p = 0;
    for (size_t i = 0; i < period.size(); ++i)
    {
        p |= (1 << ((period[i]-'0') % 7));
    }
    return p;
}

DateSpan DateSpan::intersect(const DateSpan& rhs) const
{
    if ((period_ & rhs.period_) == 0)
    {
        return DateSpan("0000-00-00", "0000-00-00", 0);
    }
}

void DateSpan::substitute(const std::vector<DateSpan>& dps,
                          const DateSpan& newDp)
{
}

void DateSpan::merge(const std::vector<DateSpan>& in,
                     std::vector<DateSpan>* out)
{
    static const int FROM_DATE = 0x1;
    static const int TO_DATE   = 0x2;

    std::map<DateTime, char> date;

    for (std::vector<DateSpan>::const_iterator inIter = in.begin();
         inIter != in.end(); ++inIter)
    {
        date[(*inIter).fromDate()] |= FROM_DATE;
        date[(*inIter).toDate()]   |= TO_DATE;
    }

    std::map<DateTime, char>::iterator dateIter = date.begin();
    std::map<DateTime, char>::iterator dateNextIter = date.begin();

    ++dateNextIter;
    while (dateNextIter != date.end())
    {
        DateTime currFromDate = (*dateIter).first;        
        if (((*dateIter).second & FROM_DATE) == 0)
        {
            currFromDate.addDay(1);
        }

        DateTime currToDate = (*dateNextIter).first;
        if (((*dateNextIter).second & TO_DATE) == 0)
        {
            currToDate.addDay(-1);
        }

        char currPeriod = 0;
        for (std::vector<DateSpan>::const_iterator inIter = in.begin();
             inIter != in.end(); ++inIter)
        {
            if ((*inIter).fromDate() <= currFromDate &&
                currToDate <= (*inIter).toDate())
            {
                currPeriod |= (*inIter).period();
            }
        }

        if (currPeriod != 0)
        {
            DateSpan current(currFromDate, currToDate, currPeriod);
            bool needPushed = true;
            if ((*out).size() > 0)
            {
                DateSpan& last = (*out)[(*out).size()-1];
                if (canBeMerged(last, current))
                {
                    last.setToDate(current.toDate());
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

bool DateSpan::canBeMerged(const DateSpan& first,
                           const DateSpan& second)
{
    char p1 = first.period();
    char p2 = second.period();
    char same = p1 & p2;
    char p1only = p1 ^ same;
    char p2only = p2 ^ same;
    char pboth = p1 | p2;

    if (hasEffectiveDay(first.fromDate(), first.toDate(), p2only) ||
        hasEffectiveDay(second.fromDate(), second.toDate(), p1only))
    {
        return false;
    }

    DateTime fromDate(first.toDate());
    fromDate.addDay(1);
    DateTime toDate(second.fromDate());
    toDate.addDay(-1);

    return !hasEffectiveDay(fromDate, toDate, pboth);
}

bool DateSpan::hasEffectiveDay(const DateTime& from,
                               const DateTime& to,
                               char period)
{
    if (period  == 0) return false;

    int dayNum = to.getDiffDayNum(from);
    if (dayNum >= 7) return true;

    int wday = from.getWeekday();
    for (int i = 0; i <= dayNum; i++)
    {
        if (isWeekdaySet(period, wday))
        {
            return true;
        }
        wday = (wday + 1) % 7;
    }

    return false;
}

DateSpan& DateSpan::normalize()
{
    if (period_  == 0) return *this;

    int dayShift = 0;
    int wday = toDate_.getWeekday();
    while (!isWeekdaySet(period_, wday))
    {
        wday = (wday - 1) % 7;
        dayShift--;
    }
    toDate_.addDay(dayShift);

    dayShift = 0;
    wday = fromDate_.getWeekday();
    while (!isWeekdaySet(period_, wday))
    {
        wday = (wday + 1) % 7;
        dayShift++;
    }
    fromDate_.addDay(dayShift);

    int dayNum = toDate_.getDiffDayNum(fromDate_);
    if (dayNum <= 0)
    {
        period_ = 0;
        return *this;
    }

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

