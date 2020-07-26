#ifndef __BOARD_H__
#define __BOARD_H__

#include "cfg/WiFiSettings.h"
#include "cfg/ProjectSettings.h"

class Board
{
protected:
  WiFiSettings    *_ws;
  ProjectSettings *_ps;

public:
  enum class InitError { None, SD, WIFI, LCD };

  Board(void):
    _ws(new WiFiSettings()),
    _ps(new ProjectSettings())
  {
    ; // empty
  }

  virtual InitError begin(void) =0;
  virtual bool readWiFiSettings(const char *filepath) =0;
  virtual bool connectToWiFi(void) =0;
  virtual bool isConnectedWiFi(void) =0;
  virtual int signalQualityWiFi(void) =0;
  size_t accessPointCount()
    { return nullptr != _ws ? _ws->accessPointCount() : 0; }
  AccessPoint *accessPoint(int n)
    { return nullptr != _ws ? _ws->accessPoint(n) : nullptr; }

  virtual bool readProjectSettings(const char *filepath) =0;
  size_t projectCount()
    { return nullptr != _ps ? _ps->projectCount() : 0; }
  Project *project(int n)
    { return nullptr != _ps ? _ps->project(n) : nullptr; }
  char *projectsJoinedBy(const char *sep)
    { return nullptr != _ps ? _ps->joinProjects(sep) : nullptr; }
  char *activitiesJoinedBy(int projectIndex, const char *sep)
    { return nullptr != _ps ? _ps->joinActivities(projectIndex, sep) : nullptr; }
};

extern Board *board;

#endif // __BOARD_H__
