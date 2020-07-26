#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "../../timecard.h"

#include "../time/TimeRunner.h"
#include "../Board.h"
#include "Dimensions.h"
#include "StatusBar.h"
#include "Navigator.h"

class Interface: public TimeRunner
{
private:
  Project *_project;
  Activity *_activity;

protected:
  Board     *_board;
  StatusBar  _statusBar;
  Navigator  _navigator;

public:
  Interface(Board *board);

  Project *project() { return _project; }
  Activity *activity() { return _activity; }

  void initLayout(void);

  void update(void);
  void onMinuteChange(void);
  void onSecondChange(void);
  void onDayChange(void);
  void onRegHoursWorked(void);
  void onMaxHoursWorked(void);
  void onWorkBlockChange(void);
  void onIsWorkingChange(void);

  void onProjectSelect(int selected);
  void onActivitySelect(int selected);
  void onOvertimeSwitch(bool isOn);
  void onWorkToggle(bool isOn);
};

extern Interface *interface;

static void handleProjectSelect(lv_obj_t *obj, lv_event_t event);
static void handleActivitySelect(lv_obj_t *obj, lv_event_t event);
static void handleOvertimeSwitch(lv_obj_t *obj, lv_event_t event);
static void handleWorkToggle(lv_obj_t *obj, lv_event_t event);

#endif // __INTERFACE_H__
