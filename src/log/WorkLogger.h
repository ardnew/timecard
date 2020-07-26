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

  void update(void);
  void onMinuteChange(void);
  void onSecondChange(void);
  void onDayChange(void);
  void onRegHoursWorked(void);
  void onMaxHoursWorked(void);
  void onWorkBlockChange(void);
  void onIsWorkingChange(void);
};

extern WorkLogger *workLogger;

#endif // __WORK_LOGGER_H__
