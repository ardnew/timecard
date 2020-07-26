#ifndef __TIME_RUNNER_H__
#define __TIME_RUNNER_H__

class TimeRunner
{
public:
  virtual void update(void) =0;
  virtual void onMinuteChange(void) =0;
  virtual void onSecondChange(void) =0;
  virtual void onDayChange(void) =0;
  virtual void onRegHoursWorked(void) =0;
  virtual void onMaxHoursWorked(void) =0;
  virtual void onWorkBlockChange(void) =0;
  virtual void onIsWorkingChange(void) =0;
};

#endif // __BOARD_H__
