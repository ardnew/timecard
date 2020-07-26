#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "../../timecard.h"

#include "Dimensions.h"

class Controller
{
protected:
  lv_obj_t *_timeGauge;
  lv_obj_t *_overtimeSwitch;
  lv_obj_t *_overtimeLabel;
  lv_obj_t *_workToggle;
  lv_obj_t *_workLabel;
public:
  constexpr static int32_t const hourScale = 10;
  Controller(void);
  void initLayout(lv_obj_t *parent);
  void update(void);
  void onOvertimeSwitch(bool isOn);
  void onWorkToggle(bool isOn);
  void setIsWorking(bool isWorking);
  lv_obj_t *overtimeSwitch(void) { return _overtimeSwitch; }
  lv_obj_t *workToggle(void) { return _workToggle; }
  static bool isToggleOff(lv_obj_t *button)
    { return LV_BTN_STATE_RELEASED == lv_btn_get_state(button); }
  static bool isToggleOn(lv_obj_t *button)
    { return LV_BTN_STATE_CHECKED_RELEASED == lv_btn_get_state(button); }
};

static void timeGaugeLabelFormat(lv_obj_t *gauge, char *buf, int bufsize, int32_t value);

#endif // __CONTROLLER_H__
