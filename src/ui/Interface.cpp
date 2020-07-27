#include "Interface.h"

#include "../time/TimeKeeper.h"
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
  lv_obj_set_event_cb(controller->workToggle(), handleWorkToggle);

  // initialize default project and activity
  if (_board->projectCount() > 0) {
    char *projects = _board->projectsJoinedBy("\n");
    selector->setProjects(projects);
    free(projects);
    onProjectSelect(0);
  }
}

void Interface::update(void)
{
  static uint32_t lastMinutesWorked = 0;

  // update content views
  _navigator.update();

  if (nullptr != timeKeeper) {
    uint32_t currMinutesWorked = timeKeeper->minutesWorked();
    if (lastMinutesWorked != currMinutesWorked) {
      lastMinutesWorked = currMinutesWorked;
      Controller *controller = _navigator.controller();
      if (nullptr != controller) {
        controller->setWorked(currMinutesWorked);
      }
    }
  }
}

void Interface::onDayChange(void)
{
  ; // empty
}

void Interface::onMinuteChange(void)
{
  ; // empty
}

void Interface::onSecondChange(void)
{
  if (nullptr != timeKeeper) {
    char *currentTime = timeKeeper->currentTime("M j, H:i:s");
    _statusBar.updateTime(currentTime);
    free(currentTime);
  }

  _statusBar.updateWiFi(_board->isConnectedWiFi(), _board->signalQualityWiFi());
}

void Interface::onRegHoursWorked(void)
{
  if (!isOvertimeEnabled()) {
    if (nullptr != timeKeeper) {
      timeKeeper->setIsWorking(false);
    }
  }
}

void Interface::onMaxHoursWorked(void)
{
  if (nullptr != timeKeeper) {
    timeKeeper->setIsWorking(false);
  }
}

void Interface::onWorkBlockChange(void)
{
  ;
}

void Interface::onIsWorkingChange(void)
{
  if (nullptr != timeKeeper) {
    Controller *controller = _navigator.controller();
    if (nullptr != controller) {
      controller->setIsWorking(timeKeeper->isWorking());
    }
  }
}

void Interface::onProjectSelect(int selected)
{
  if (nullptr != timeKeeper) {
    timeKeeper->setIsWorking(false);
  }
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
  if (nullptr != timeKeeper) {
    timeKeeper->setIsWorking(false);
  }
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

void Interface::onWorkToggle(bool isOn)
{
  Controller *controller = _navigator.controller();
  if (nullptr != controller) {
    controller->onWorkToggle(isOn);
  }
  if (nullptr != timeKeeper) {
    timeKeeper->setIsWorking(isOn);
  }
}

bool Interface::isOvertimeEnabled(void)
{
  Controller *controller = _navigator.controller();
  if (nullptr != controller) {
    return Controller::isSwitchOn(controller->overtimeSwitch());
  }
  return false;
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
      interface->onOvertimeSwitch(Controller::isSwitchOn(obj));
    }
  }
}

static void handleWorkToggle(lv_obj_t *obj, lv_event_t event)
{
  if (nullptr != interface) {
    if (LV_EVENT_VALUE_CHANGED == event) {
      interface->onWorkToggle(Controller::isToggleOn(obj));
    }
  }
}
