#include "WiFiSettings.h"

AccessPoint::AccessPoint(void):
  _ssid(nullptr),
  _pass(nullptr)
{
  ; // empty
}

AccessPoint::AccessPoint(const char *ssid, const char *pass):
  AccessPoint()
{
  if (nullptr != ssid) {
    size_t len = strlen(ssid);
    if (len > 0) {
      _ssid = (char *)calloc(len + 1, sizeof(*ssid));
      _ssid = (char *)strncpy(_ssid, ssid, len);
    }
  }
  if (nullptr != pass) {
    size_t len = strlen(pass);
    if (len > 0) {
      _pass = (char *)calloc(len + 1, sizeof(*pass));
      _pass = (char *)strncpy(_pass, pass, len);
    }
  }
}

char *AccessPoint::ssid() { return _ssid; }
char *AccessPoint::pass() { return _pass; }

WiFiSettings::WiFiSettings(void):
  _ap(new LinkedList<AccessPoint *>())
{
  ; // empty
}

bool WiFiSettings::loadStream(ReadBufferingStream &stream)
{
  DynamicJsonDocument doc(2*1024);
  DeserializationError error = deserializeJson(doc, stream);
  if (error) {
    return false;
  }
  for (JsonVariant ap : doc["ap"].as<JsonArray>()) {
    const char *ssid = ap["ssid"];
    const char *pass = ap["pass"];
    _ap->add(new AccessPoint(ssid, pass));
  }
  return true;
}

size_t WiFiSettings::accessPointCount()
  { return nullptr != _ap ? _ap->size() : 0; }

AccessPoint *WiFiSettings::accessPoint(int n)
  { return n < accessPointCount() ? _ap->get(n) : nullptr; }
