#include "Interface.h"

#include "Selector.h"
#include "Controller.h"

Interface *interface;

Interface::Interface(Board *board):
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

  if (_board->projectCount() > 0) {

    char *projects = _board->projectsJoinedBy("\n");
    selector->setProjects(projects);
    free(projects);

    char *activities = _board->activitiesJoinedBy(0, "\n");
    selector->setActivities(activities);
    free(activities);
  }

  // force statusbar event handlers to fire (don't wait for time to elapse)
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
