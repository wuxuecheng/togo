#include "DateSpan.h"
#include <map>
#include <vector>

#define CROL(value, width, bits) ((value << bits) | (value >> (width - bits)))
#define CROR(value, width, bits) ((value >> bits) | (value << (width - bits)))

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
    DateTime newFromDate(rhs.fromDate_ < rhs.fromDate_ ?
                         fromDate_ : rhs.fromDate_);
    DateTime newToDate(toDate_ < rhs.toDate_ ? toDate_ : rhs.toDate_);
    char newPeriod = period_ & rhs.period_;
    return DateSpan(newFromDate, newToDate, newPeriod);
}

void DateSpan::substitute(const std::vector<DateSpan>& in,
                          const DateSpan& newDp,
                          std::vector<DateSpan>* out)
{
    (*out).push_back(newDp);

    for (std::vector<DateSpan>::const_iterator inIter = in.begin();
         inIter != in.end(); ++inIter)
    {
        //  +++ 代表的日期段是我们想要的
        // *inIter  ++++++++
        // newDp               --------
        if ((*inIter).toDate() < newDp.fromDate() ||
            newDp.toDate() < (*inIter).fromDate())
        {
            (*out).push_back(*inIter);
        }
        else
        {
            // *inIter ++++++-------
            // newDp         -------------
            if ((*inIter).fromDate() < newDp.fromDate() &&
                newDp.fromDate() < (*inIter).toDate())
            {
                DateTime toDate(newDp.fromDate());
                toDate.addDay(-1);
                (*out).push_back(DateSpan((*inIter).fromDate(),
                                          toDate,
                                          (*inIter).period()));
            }
            // *inIter         -----+++++++
            // newDp   -------------
            if ((*inIter).fromDate() < newDp.toDate() &&
                newDp.toDate() < (*inIter).toDate())
            {
                DateTime fromDate(newDp.toDate());
                fromDate.addDay(1);
                (*out).push_back(DateSpan(fromDate,
                                          (*inIter).toDate(),
                                          (*inIter).period()));
            }
        }
    }        
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
        // 该日期不是起始日期，则需要后移1天  
        if (((*dateIter).second & FROM_DATE) == 0)
        {
            currFromDate.addDay(1);
        }

        DateTime currToDate = (*dateNextIter).first;
        // 该日期不是终止日期，则需要前移1天
        if (((*dateNextIter).second & TO_DATE) == 0)
        {
            currToDate.addDay(-1);
        }

        if (currToDate < currFromDate)
        {
            dateIter++;
            dateNextIter++;
            continue;
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
    if (period_  == 0)
    {
        fromDate_ = DateTime("0000-00-00");
        toDate_ = DateTime("0000-00-00");
        return *this;
    }
    // 调整起始日期
    int dayShift = 0;
    int wday = toDate_.getWeekday();
    while (!isWeekdaySet(period_, wday))
    {
        dayShift--;
        wday--;
        if (wday == 0)
            wday = 7;        
    }
    toDate_.addDay(dayShift);
    // 调整终止日期
    dayShift = 0;
    wday = fromDate_.getWeekday();
    while (!isWeekdaySet(period_, wday))
    {
        wday = (wday + 1) % 7;
        dayShift++;
    }
    fromDate_.addDay(dayShift);

    int dayNum = toDate_.getDiffDayNum(fromDate_);
    if (dayNum < 0)
    {
        fromDate_ = DateTime("0000-00-00");
        toDate_ = DateTime("0000-00-00");
        period_ = 0;
        return *this;
    }

    // 如果日期数小于7，则需调整period
    if (dayNum < 7)
    {
        char effective = 0;
        for (int i = 0; i <= dayNum; i++)
        {
            effective |= (1 << wday);
            wday = (wday + 1) % 7;
        }
        period_ &= effective;
    }
    return *this;
}

DateSpan& DateSpan::shiftDays(int dayNum)
{
    if (dayNum != 0)
    {
        fromDate_.addDay(dayNum);
        toDate_.addDay(dayNum);
        if (dayNum < 0)
        {
            dayNum = (dayNum % 7) + 7;
        }
        period_ = CROL(period_, 7, dayNum);
    }
    return *this;
}

