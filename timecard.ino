#include "timecard.h"

#include "src/Board.h"
#ifdef ADAFRUIT_PYPORTAL
#include "src/hw/PyPortal.h"
#endif

#include "src/time/TimeKeeper.h"
#include "src/log/WorkLogger.h"
#include "src/ui/Interface.h"

void setup(void)
{
  WAIT_FOR_SERIAL(10000, 115200);

  switch (board->begin()) {
    case Board::InitError::None: {

      if (board->loadWiFiSettings(FILE_WIFI_JSON)) {
        bool isConnected = board->connectToWiFi();
        if (isConnected) {
          infof("%s", "connected!");
        } else {
          errf("%s", "failed to connect to any access point");
        }
      } else {
        errf("%s", "failed to read WiFi settings");
      }

      timeKeeper->initLocalTime(CLOCK_TIMEZONE);

      if (board->loadProjectSettings(FILE_PROJECTS_JSON)) {
        infof("found %d projects:", board->projectCount());
        for (int i = 0; i < board->projectCount(); ++i) {
          Project *p = board->project(i);
          infof("\t%s (%d activities)", p->name(), p->activityCount());
        }
      } else {
        errf("%s", "failed to read project settings");
      }

      workLogger = new WorkLogger(board);
      timeKeeper->add(workLogger);

      interface = new Interface(board);
      timeKeeper->add(interface);

      // if we restarted, load the total minutes already logged.
      uint32_t minutesLogged;
      if (workLogger->totalMinutesWorked(&minutesLogged)) {
        timeKeeper->setMinutesWorked(minutesLogged);
      }

      break;
    }
    case Board::InitError::SD: {
      errf("%s", "failed to initialize SD module");
      break;
    }
    case Board::InitError::WIFI: {
      errf("%s", "failed to initialize Wi-Fi module");
      break;
    }
    case Board::InitError::LCD: {
      errf("%s", "failed to initialize touchscreen LCD");
      break;
    }
  }
}

void loop(void)
{
  // fire the ezTime events loop
  events();

  // fire the LvGL events loop
  lv_task_handler();

  timeKeeper->update();

  delay(CPU_SLEEP_DELAY);
}

void print(info_level_t level, const char *fmt, ...)
{
#ifdef PRINTF_DEBUG
  static const char *DEBUG_LEVEL_PREFIX[ilCOUNT] = {
    "[ ] ", "[*] ", "[!] "
  };
  static char buff[PRINTF_DEBUG_MAX_LEN] = { 0 };

  va_list arg;
  va_start(arg, fmt);
  vsnprintf(buff, PRINTF_DEBUG_MAX_LEN, fmt, arg);
  va_end(arg);

  Serial.print(DEBUG_LEVEL_PREFIX[level]);
  Serial.println(buff);
#else
  (void)level;
  (void)fmt;
#endif
}
