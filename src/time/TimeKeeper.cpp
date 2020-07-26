#include "TimeKeeper.h"

TimeKeeper *timeKeeper = new TimeKeeper();

// flag indicating our first iteration through Arduino's loop()
static bool isFirstLoop = true;

// forward-declare private static functions for greedy hermit points.
static char *makeCString(String str) // be sure to free() after use!
{
  int len = str.length();
  char *cstr = (char *)calloc(len+1, sizeof(char));
  str.toCharArray(cstr, len+1);
  return cstr;
}

TimeKeeper::TimeKeeper(void):
  _clock(new Clock()),
  _runs(new LinkedList<TimeRunner *>()),
  _daySlot(0x7F),
  _workSlot(0x7F),
  _isWorking(false),
  _minutesWorked(0),
  _timeStamp()
{
  ; // empty
}

void TimeKeeper::update(void)
{
  if (isFirstLoop) {
    // force timer event handler one time at initialization
    onMinuteChange();
    isFirstLoop = false;
  } else {
    if      (minuteChanged()) { onMinuteChange(); }
    else if (secondChanged()) { onSecondChange(); }
  }
  if (nullptr != _runs) {
    for (int i = 0; i < _runs->size(); ++i) {
      TimeRunner *run = _runs->get(i);
      if (nullptr != run) {
        run->update();
      }
    }
  }
}

void TimeKeeper::initLocalTime(const char *tz)
{
  synchronizeTime();
  setTimezone(tz);

  if (nullptr != _clock) {
    _timeStamp = TimeStamp(_clock->now());
  }

  char *currentTime = timeKeeper->currentTime();
  infof("%s:", "current time");
  infof("\t%s", currentTime);
  free(currentTime);
}

void TimeKeeper::synchronizeTime(void)
{
  if (nullptr != _clock) {
    _clock->synchronize();
  }
}

void TimeKeeper::setTimezone(const char *tz)
{
  if (nullptr != _clock) {
    _clock->setTimezone(tz);
  }
}

char *TimeKeeper::currentTime(void)
{
  if (nullptr != _clock) {
    return makeCString(_clock->dateTime());
  } else {
    return nullptr;
  }
}

char *TimeKeeper::currentTime(const char *format)
{
  if (nullptr != _clock) {
    return makeCString(_clock->dateTime(String(format)));
  } else {
    return nullptr;
  }
}

time_t TimeKeeper::secondsAfterNow(int seconds)
{
  if (nullptr != _clock) {
    return (time_t)((int32_t)(_clock->now()) + (int32_t)seconds);
  } else {
    return (time_t)((int32_t)seconds);
  }
}

time_t TimeKeeper::minutesAfterNow(int minutes)
{
  return secondsAfterNow(minutes * 60);
}

void TimeKeeper::add(TimeRunner *runner)
{
  if (nullptr != _runs && nullptr != runner) {
    _runs->add(runner);
  }
}

void TimeKeeper::onDayChange(void)
{
  if (nullptr != _runs) {
    for (int i = 0; i < _runs->size(); ++i) {
      TimeRunner *run = _runs->get(i);
      if (nullptr != run) {
        run->onDayChange();
        run->onMinuteChange();
        run->onSecondChange();
      }
    }
  }
}

void TimeKeeper::onMinuteChange(void)
{
  if (nullptr != _clock) {
    TimeStamp now = TimeStamp(_clock->now());
    bool didDayChange = _timeStamp.didDayChange(now);
    _timeStamp = now; // be sure to set before calling on*Change event handlers.
    if (didDayChange) {
      onDayChange(); // also calls onMinuteChange, onSecondChange
    } else {
      if (nullptr != _runs) {
        for (int i = 0; i < _runs->size(); ++i) {
          TimeRunner *run = _runs->get(i);
          if (nullptr != run) {
            run->onMinuteChange();
            run->onSecondChange();
          }
        }
      }
    }
  }
}

void TimeKeeper::onSecondChange(void)
{
  if (nullptr != _runs) {
    for (int i = 0; i < _runs->size(); ++i) {
      TimeRunner *run = _runs->get(i);
      if (nullptr != run) {
        run->onSecondChange();
      }
    }
  }
}

void TimeKeeper::onRegHoursWorked(void)
{
  if (nullptr != _runs) {
    for (int i = 0; i < _runs->size(); ++i) {
      TimeRunner *run = _runs->get(i);
      if (nullptr != run) {
        run->onRegHoursWorked();
      }
    }
  }
}

void TimeKeeper::onMaxHoursWorked(void)
{
  if (nullptr != _runs) {
    for (int i = 0; i < _runs->size(); ++i) {
      TimeRunner *run = _runs->get(i);
      if (nullptr != run) {
        run->onMaxHoursWorked();
      }
    }
  }
}

void TimeKeeper::onWorkBlockChange(void)
{
  if (nullptr != _runs) {
    for (int i = 0; i < _runs->size(); ++i) {
      TimeRunner *run = _runs->get(i);
      if (nullptr != run) {
        run->onWorkBlockChange();
      }
    }
  }
  if (_isWorking) {
    _workSlot = setEvent(handleWorkBlockChange, secondsAfterNow(WORK_BLOCK_INTERVAL));
  }
}

void TimeKeeper::onIsWorkingChange(void)
{
  if (nullptr != _runs) {
    for (int i = 0; i < _runs->size(); ++i) {
      TimeRunner *run = _runs->get(i);
      if (nullptr != run) {
        run->onIsWorkingChange();
      }
    }
  }
}

void TimeKeeper::setIsWorking(bool isWorking)
{
  if (_isWorking != isWorking) {
    _isWorking = isWorking;
    onIsWorkingChange();
    if (isWorking) {
      _workSlot = setEvent(handleWorkBlockChange, secondsAfterNow(WORK_BLOCK_INTERVAL));
    } else {
      deleteEvent(_workSlot);
    }
  }
}

static void handleWorkBlockChange(void)
{
  if (nullptr != timeKeeper) {
    timeKeeper->onWorkBlockChange();
  }
}
