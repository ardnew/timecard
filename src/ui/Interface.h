#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "../../timecard.h"

#include "../Board.h"
#include "Dimensions.h"
#include "StatusBar.h"
#include "Navigator.h"

class Interface
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

  void update(void);
  void initLayout(void);
  void onSecondChange(void);
  void onMinuteChange(void);
  void onProjectSelect(int selected);
  void onActivitySelect(int selected);
  void onOvertimeSwitch(bool isOn);
  void onRestartToggle(bool isOn);
};

extern Interface *interface;

static void handleProjectSelect(lv_obj_t *obj, lv_event_t event);
static void handleActivitySelect(lv_obj_t *obj, lv_event_t event);
static void handleOvertimeSwitch(lv_obj_t *obj, lv_event_t event);
static void handleRestartToggle(lv_obj_t *obj, lv_event_t event);

#endif // __INTERFACE_H__
