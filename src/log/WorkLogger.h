#ifndef __WORK_LOGGER_H__
#define __WORK_LOGGER_H__

#include "../../timecard.h"

#include "../time/TimeRunner.h"
#include "../Board.h"

class WorkLogger: public TimeRunner
{
protected:
  Board *_board;
  void logWork(void);

public:
  WorkLogger(Board *board);

  bool totalMinutesWorked(uint32_t *minutes);

  void update(void);
  void onMinuteChange(void);
  void onSecondChange(void);
  void onDayChange(void);
  void onRegHoursWorked(void);
  void onMaxHoursWorked(void);
  void onWorkBlockChange(void);
  void onIsWorkingChange(void);

  static uint32_t parseMinutes(char *line);

  static uint digitCount(uint n) {
    return (uint)(floorf(log10f((float)n))) + 1;
  }
};

extern WorkLogger *workLogger;

#endif // __WORK_LOGGER_H__
