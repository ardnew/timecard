#include "Controller.h"

Controller::Controller(void):
  _timeGauge(nullptr),
  _overtimeSwitch(nullptr),
  _overtimeLabel(nullptr),
  _restartToggle(nullptr),
  _restartLabel(nullptr)
{
  ; // empty
}

void Controller::initLayout(lv_obj_t *parent)
{
  _overtimeSwitch = lv_switch_create(parent, NULL);
  lv_obj_align(_overtimeSwitch, NULL, LV_ALIGN_IN_TOP_RIGHT, OVERTIME_H_ALIGN, OVERTIME_V_ALIGN);

  static lv_style_t labelStyle;
  lv_style_init(&labelStyle);
  lv_style_set_text_font(&labelStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12);

  _overtimeLabel = lv_label_create(parent, NULL);
  //lv_obj_set_width(_overtimeLabel, 50);
  lv_obj_add_style(_overtimeLabel, LV_OBJ_PART_MAIN, &labelStyle);
  lv_label_set_text(_overtimeLabel, "EWW");
  lv_obj_align(_overtimeLabel, _overtimeSwitch, LV_ALIGN_OUT_LEFT_MID, -OVERTIME_LABEL_SPACING, 0);

  static lv_color_t gaugeColor[1];
  gaugeColor[0] = LV_COLOR_WHITE;

  _timeGauge = lv_gauge_create(parent, NULL);
  lv_gauge_set_needle_count(_timeGauge, sizeof_array(gaugeColor), gaugeColor);
  lv_obj_set_size(_timeGauge, TIME_GAUGE_WIDTH, TIME_GAUGE_HEIGHT);
  lv_obj_align(_timeGauge, NULL, LV_ALIGN_CENTER, TIME_GAUGE_H_ALIGN, TIME_GAUGE_V_ALIGN);

  lv_gauge_set_scale(_timeGauge, TIME_GAUGE_ANGLE, TIME_GAUGE_TICK_COUNT, TIME_GAUGE_LABEL_COUNT);
  lv_gauge_set_range(_timeGauge, 0, MAX_HOURS_PER_DAY * Controller::hourScale);
  lv_gauge_set_critical_value(_timeGauge, REG_HOURS_PER_DAY * Controller::hourScale);
  lv_gauge_set_formatter_cb(_timeGauge, timeGaugeLabelFormat);

  lv_gauge_set_value(_timeGauge, 0, 0);

  _restartToggle = lv_btn_create(_timeGauge, NULL);
  lv_obj_align(_restartToggle, NULL, LV_ALIGN_CENTER, 0, RESTART_TOGGLE_V_ALIGN);
  lv_btn_set_checkable(_restartToggle, true);
  lv_btn_set_fit2(_restartToggle, LV_FIT_TIGHT, LV_FIT_TIGHT);

  _restartLabel = lv_label_create(_restartToggle, NULL);
  lv_obj_align(_restartLabel, NULL, LV_ALIGN_CENTER, 0, 0);

  onOvertimeSwitch(false);
  onRestartToggle(false);
}

void Controller::update(void)
{
  ; // empty
}

void Controller::onOvertimeSwitch(bool isOn)
{
  // TBD
}

void Controller::onRestartToggle(bool isOn)
{
  if (isOn) {
    lv_label_set_text(_restartLabel, LV_SYMBOL_PAUSE);
  } else {
    lv_label_set_text(_restartLabel, LV_SYMBOL_PLAY);
  }
}

static void timeGaugeLabelFormat(lv_obj_t *gauge, char *buf, int bufsize, int32_t value)
{
  snprintf(buf, bufsize, "%d", value / Controller::hourScale);
}
