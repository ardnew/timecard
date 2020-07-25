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
  lv_obj_t *_restartToggle;
  lv_obj_t *_restartLabel;
public:
  constexpr static int32_t const hourScale = 10;
  Controller(void);
  void initLayout(lv_obj_t *parent);
  void update(void);
  void onOvertimeSwitch(bool isOn);
  void onRestartToggle(bool isOn);
  lv_obj_t *overtimeSwitch(void) { return _overtimeSwitch; }
  lv_obj_t *restartToggle(void) { return _restartToggle; }
};

static void timeGaugeLabelFormat(lv_obj_t *gauge, char *buf, int bufsize, int32_t value);

#endif // __CONTROLLER_H__
