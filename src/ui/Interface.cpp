#include "Interface.h"

#include "Selector.h"
#include "Controller.h"

Interface *interface;

Interface::Interface(Board *board):
  _project(nullptr),
  _activity(nullptr),
  _board(board),
  _statusBar(),
  _navigator()
{
  initLayout();
}

void Interface::initLayout(void)
{
  _statusBar.initLayout(lv_scr_act());
  _navigator.initLayout(lv_scr_act());

  Selector *selector = _navigator.selector();
  Controller *controller = _navigator.controller();

  // register callback handlers
  lv_obj_set_event_cb(selector->projectList(), handleProjectSelect);
  lv_obj_set_event_cb(selector->activityRoller(), handleActivitySelect);
  lv_obj_set_event_cb(controller->overtimeSwitch(), handleOvertimeSwitch);
  lv_obj_set_event_cb(controller->restartToggle(), handleRestartToggle);

  // initialize default project and activity
  if (_board->projectCount() > 0) {
    char *projects = _board->projectsJoinedBy("\n");
    selector->setProjects(projects);
    free(projects);
    onProjectSelect(0);
  }

  // force statusbar event handlers to fire
  onMinuteChange(); // calls onSecondChange()
}

void Interface::update(void)
{
  // fire the ezTime events loop
  events();

  // fire the LvGL events loop
  lv_task_handler();

  // update statusbar
  if (minuteChanged())      { onMinuteChange(); }
  else if (secondChanged()) { onSecondChange(); }

  // update content views
  _navigator.update();

  delay(CPU_SLEEP_DELAY);
}

void Interface::onSecondChange(void)
{
  char *currentTime = _board->currentTime("M j, H:i:s");
  _statusBar.updateTime(currentTime);
  free(currentTime);

  _statusBar.updateWiFi(_board->isConnectedWiFi(), _board->signalQualityWiFi());
}

void Interface::onMinuteChange(void)
{
  onSecondChange();
}

void Interface::onProjectSelect(int selected)
{
  if (nullptr != _board) {
    _project = _board->project(selected);
    if (nullptr != _project && _project->activityCount() > 0) {
      Selector *selector = _navigator.selector();
      if (nullptr != selector) {
        char *activities = _board->activitiesJoinedBy(selected, "\n");
        selector->setActivities(activities);
        free(activities);
      }
      infof("selected project: %s", _project->name());
      onActivitySelect(0);
    }
  }
}

void Interface::onActivitySelect(int selected)
{
  if (nullptr != _project) {
    _activity = _project->activity(selected);
    if (nullptr != _activity) {
      infof("selected activity: (%s) %s", _activity->id(), _activity->desc());
    }
  }
}

void Interface::onOvertimeSwitch(bool isOn)
{
  Controller *controller = _navigator.controller();
  if (nullptr != controller) {
    controller->onOvertimeSwitch(isOn);
  }
}

void Interface::onRestartToggle(bool isOn)
{
  Controller *controller = _navigator.controller();
  if (nullptr != controller) {
    controller->onRestartToggle(isOn);
  }
}

static void handleProjectSelect(lv_obj_t *obj, lv_event_t event)
{
  if (nullptr != interface) {
    if (LV_EVENT_VALUE_CHANGED == event) {
      interface->onProjectSelect((int)lv_dropdown_get_selected(obj));
    }
  }
}

static void handleActivitySelect(lv_obj_t *obj, lv_event_t event)
{
  if (nullptr != interface) {
    if (LV_EVENT_VALUE_CHANGED == event) {
      interface->onActivitySelect((int)lv_roller_get_selected(obj));
    }
  }
}

static void handleOvertimeSwitch(lv_obj_t *obj, lv_event_t event)
{
  if (nullptr != interface) {
    if (LV_EVENT_VALUE_CHANGED == event) {
      interface->onOvertimeSwitch(lv_switch_get_state(obj));
    }
  }
}

static void handleRestartToggle(lv_obj_t *obj, lv_event_t event)
{
  if (nullptr != interface) {
    if (LV_EVENT_VALUE_CHANGED == event) {
      interface->onRestartToggle(LV_BTN_STATE_CHECKED_RELEASED == lv_btn_get_state(obj));
    }
  }
}
