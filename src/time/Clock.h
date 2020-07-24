#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "../../timecard.h"

class Clock
{
protected:
  Timezone *_tz;
public:
  Clock(void);
  void setTimezone(const char *tz);
  void synchronize(void);
  String dateTime(void);
  String dateTime(const String format);
  time_t now(void);
};

#endif // __CLOCK_H__
