#ifndef __TIME_KEEPER_H__
#define __TIME_KEEPER_H__

#include "../../timecard.h"

#include "../util/LinkedList.h"
#include "Clock.h"
#include "TimeRunner.h"

class TimeStamp
{
protected:
  uint8_t _minute;
  uint8_t _hour;
  uint8_t _day;
  uint8_t _month;
  uint8_t _year;
public:
  TimeStamp(void): _minute(0), _hour(0), _day(0), _month(0), _year(0) { }
  TimeStamp(time_t time)
  {
    tmElements_t elem;
    breakTime(time, elem);
    _minute = elem.Minute;
    _hour   = elem.Hour;
    _day    = elem.Day;
    _month  = elem.Month;
    _year   = elem.Year;
  }
  time_t   time()   const { return makeTime(_hour, _minute, 0, _day, _month, _year); }
  uint8_t  minute() const { return _minute; }
  uint8_t  hour()   const { return _hour; }
  uint8_t  day()    const { return _day; }
  uint8_t  month()  const { return _month; }
  uint16_t year()   const { return _year + 1970; }
  bool didMinuteChange(const TimeStamp &ts) const { return _minute != ts._minute; }
  bool didHourChange(const TimeStamp &ts)   const { return _hour != ts._hour; }
  bool didDayChange(const TimeStamp &ts)    const { return _day != ts._day; }
  bool didMonthChange(const TimeStamp &ts)  const { return _month != ts._month; }
  bool didYearChange(const TimeStamp &ts)   const { return _year != ts._year; }
};

class TimeKeeper
{
protected:
  Clock *_clock;
  LinkedList<TimeRunner *> *_runs;
  uint8_t _daySlot;
  uint8_t _workSlot;
  bool _isWorking;
  uint32_t _minutesWorked;
  TimeStamp _timeStamp;

public:
  TimeKeeper(void);

  void update(void);
  void initLocalTime(const char *tz);
  void synchronizeTime(void);
  void setTimezone(const char *tz);
  char *currentTime(void);
  char *currentTime(const char *format);
  time_t secondsAfterNow(int seconds);
  time_t minutesAfterNow(int minutes);
  void add(TimeRunner *runner);

  TimeStamp timeStamp(void) { return _timeStamp; }

  void onDayChange(void);
  void onMinuteChange(void);
  void onSecondChange(void);
  void onRegHoursWorked(void);
  void onMaxHoursWorked(void);
  void onWorkBlockChange(void);
  void onIsWorkingChange(void);

  void setIsWorking(bool isWorking);
  bool isWorking(void) { return _isWorking; }

  void setMinutesWorked(uint32_t minutes);
  uint32_t minutesWorked(void) { return _minutesWorked; }
};

extern TimeKeeper *timeKeeper;

static void handleWorkBlockChange(void);

#endif // __TIME_KEEPER_H__
