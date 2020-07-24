#ifndef __BOARD_H__
#define __BOARD_H__

#include "util/LinkedList.h"
#include "cfg/WiFiSettings.h"
#include "cfg/ProjectSettings.h"
#include "time/Clock.h"

class Board
{
private:
  char *strToChar(String str) // be sure to free() after use!
  {
    int len = str.length();
    char *cstr = (char *)calloc(len+1, sizeof(char));
    str.toCharArray(cstr, len+1);
    return cstr;
  }

protected:
  WiFiSettings    *_ws;
  ProjectSettings *_ps;
  Clock           *_clock;

public:
  enum class InitError { None, SD, WIFI, LCD };

  Board(void):
    _ws(new WiFiSettings()),
    _ps(new ProjectSettings()),
    _clock(new Clock())
  {
    ; // empty
  }

  virtual InitError begin(void) =0;

  virtual bool readWiFiSettings(const char *filepath) =0;
  virtual bool connectToWiFi(void) =0;
  virtual bool isConnectedWiFi(void) =0;
  virtual int signalQualityWiFi(void) =0;

  virtual bool readProjectSettings(const char *filepath) =0;

  size_t accessPointCount()
    { return nullptr != _ws ? _ws->accessPointCount() : 0; }
  AccessPoint *accessPoint(int n)
    { return nullptr != _ws ? _ws->accessPoint(n) : nullptr; }

  size_t projectCount()
    { return nullptr != _ps ? _ps->projectCount() : 0; }
  Project *project(int n)
    { return nullptr != _ps ? _ps->project(n) : nullptr; }
  char *projectsJoinedBy(const char *sep)
    { return nullptr != _ps ? _ps->joinProjects(sep) : nullptr; }
  char *activitiesJoinedBy(int projectIndex, const char *sep)
    { return nullptr != _ps ? _ps->joinActivities(projectIndex, sep) : nullptr; }

  void setTimezone(const char *tz)
    { if (nullptr != _clock) { _clock->setTimezone(tz); } }
  void synchronizeTime(void)
    { if (nullptr != _clock) { _clock->synchronize(); } }
  char *currentTime(void)
    { return nullptr != _clock ? strToChar(_clock->dateTime()) : nullptr; }
  char *currentTime(const char *format)
    { return nullptr != _clock ? strToChar(_clock->dateTime(String(format))) : nullptr; }
};

extern Board *board;

#endif // __BOARD_H__
