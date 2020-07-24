#include "Clock.h"

Clock::Clock(void):
  _tz(new Timezone())
{
  ; // empty
}

void Clock::setTimezone(const char *tz)
{
  _tz->setLocation(tz);
}

void Clock::synchronize(void)
{
  waitForSync();
}

String Clock::dateTime(void)
{
  return _tz->dateTime();
}

String Clock::dateTime(const String format)
{
  return _tz->dateTime(format);
}

time_t Clock::now(void)
{
  return _tz->now();
}
