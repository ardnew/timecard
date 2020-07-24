#ifndef __WIFI_SETTINGS_H__
#define __WIFI_SETTINGS_H__

#include "../util/LinkedList.h"
#include "../../timecard.h"

class AccessPoint
{
protected:
  char *_ssid;
  char *_pass;
public:
  AccessPoint(void);
  AccessPoint(const char *ssid, const char *pass);
  char *ssid();
  char *pass();
};

class WiFiSettings
{
protected:
  LinkedList<AccessPoint *> *_ap;
public:
  WiFiSettings(void);
  bool loadStream(ReadBufferingStream &stream);
  size_t accessPointCount();
  AccessPoint *accessPoint(int n);
};

#endif // __WIFI_SETTINGS_H__
