#include "PyPortal.h"

Board *board = new PyPortal();

PyPortal::PyPortal(void):
  _statusWiFi(WL_IDLE_STATUS),
  _sd(new SdFat()),
  _wifi(&WiFi),
  _tft(new Adafruit_ILI9341(tft8bitbus, PIN_TFT_D0, PIN_TFT_WR, PIN_TFT_DC, PIN_TFT_CS, PIN_TFT_RST, PIN_TFT_RD)),
  _ts(new TouchScreen(PIN_TS_XP, PIN_TS_YP, PIN_TS_XM, PIN_TS_YM, 300)),
  _glue(new Adafruit_LvGL_Glue())
{
  ; // empty
}

Board::InitError PyPortal::begin(void)
{
  if (!initSd())   { return Board::InitError::SD; }
  if (!initWifi()) { return Board::InitError::WIFI; }
  if (!initLcd())  { return Board::InitError::LCD; }

  return InitError::None;
}

bool PyPortal::readWiFiSettings(const char *filepath)
{
  if (nullptr == filepath) {
    return false;
  }
  File file = _sd->open(filepath);
  ReadBufferingStream stream(file, PARSE_BUF_SZ);
  bool success = _ws->loadStream(stream);
  file.close();
  return success;
}

bool PyPortal::connectToWiFi(void)
{
  for (int i = 0; !isConnectedWiFi() && i < board->accessPointCount(); ++i) {
    AccessPoint *ap = board->accessPoint(i);
    infof("%s", "connecting to access point:");
    infof("\tSSID: %s", ap->ssid());
    infof("\tPass: %s", ap->pass());
    _statusWiFi = _wifi->begin(ap->ssid(), ap->pass());
  }

  return isConnectedWiFi();
}

bool PyPortal::isConnectedWiFi(void)
{
  return WL_CONNECTED == _statusWiFi;
}

int PyPortal::signalQualityWiFi(void)
{
  if (isConnectedWiFi()) {
    return 2 * (_wifi->RSSI() + 100);
  } else {
    return 0;
  }
}

bool PyPortal::readProjectSettings(const char *filepath)
{
  if (nullptr == filepath) {
    return false;
  }
  File file = _sd->open(filepath);
  ReadBufferingStream stream(file, PARSE_BUF_SZ);
  bool success = _ps->loadStream(stream);
  file.close();
  return success;
}

bool PyPortal::initSd()
{
  if (nullptr == _sd || !_sd->begin(PIN_SD_CS)) {
    return false;
  }

  return true;
}

bool PyPortal::initWifi()
{
  if (nullptr == _wifi || WL_NO_MODULE == _wifi->status()) {
    return false;
  }

  return true;
}

bool PyPortal::initLcd()
{
  if (nullptr == _tft || nullptr == _ts) {
    return false;
  }

  _tft->begin();
  _tft->setRotation(TFT_ROTATION);

  pinMode(PIN_TFT_BKL, OUTPUT);
  digitalWrite(PIN_TFT_BKL, HIGH);

  return bindGfx();
}

bool PyPortal::bindGfx()
{
  if (nullptr == _glue || LVGL_OK != _glue->begin(_tft, _ts)) {
    return false;
  }
  return true;
}
