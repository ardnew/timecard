#include "PyPortal.h"

#include "../log/WorkLogger.h"

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

bool PyPortal::loadWiFiSettings(const char *filepath)
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

bool PyPortal::loadProjectSettings(const char *filepath)
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

bool PyPortal::assertWorkLog(char **filepath, const TimeStamp &timeStamp)
{
  static char yearStr[5], monthStr[3], dayStr[3];
  static const char extension[] = "log";
  static const size_t dirLen = 1 + sizeof(yearStr) + 1 + sizeof(monthStr);
  static const size_t pathLen = dirLen + 1 + sizeof(dayStr) + 1 + sizeof(extension);
  static char dir[dirLen + 1];   // e.g.: "/2020/07"
  static char path[pathLen + 1]; //       "/2020/07/13.log"

  // initialize buffers
  memset(dir, 0, dirLen + 1);
  memset(path, 0, pathLen + 1);

  // construct components and paths
  snprintf(yearStr,  sizeof(yearStr),  "%04d",     timeStamp.year());
  snprintf(monthStr, sizeof(monthStr), "%02d",     timeStamp.month());
  snprintf(dayStr,   sizeof(dayStr),   "%02d",     timeStamp.day());
  snprintf(dir,      dirLen+1,         "/%s/%s",   yearStr, monthStr);
  snprintf(path,     pathLen+1,        "%s/%s.%s", dir, dayStr, extension);

  // copy path to output - be sure to free after use!
  *filepath = (char *)calloc(pathLen+1, sizeof(**filepath));
  *filepath = strncpy(*filepath, path, pathLen);

  if (_sd->exists(path)) {
    //_sd->remove(path);
    return true; // file exists, no need to continue
  }

  // file does not exist, check if parent directory exists.
  if (!_sd->exists(dir)) {
    // create parent directory (and all of its parent directories)
    if (!_sd->mkdir(dir, true)) {
      return false; // couldn't create parent directory
    }
  }

  // parent directory exists, create file
  File file = _sd->open(path, O_WRITE | O_CREAT);
  if (!file.isOpen()) {
    return false; // failed to create file
  }
  return file.close(); // ensure file is closed before returning for usage.
}

bool PyPortal::appendWorkLogEntry(char *filepath, char *entry)
{
  infof("log(%s) -> %s", filepath, entry);
  File file = _sd->open(filepath, O_WRITE | O_AT_END);
  if (file.isOpen()) {
    file.println(entry);
    bool ok = file.sync() && !file.getWriteError();
    return file.close() && ok;
  }
  return false;
}

bool PyPortal::parseWorkLogMinutes(uint32_t *minutes, char *filepath)
{
  File file = _sd->open(filepath);
  ReadBufferingStream stream(file, PARSE_BUF_SZ);
  String buffer;

  *minutes = 0;
  while (stream.available()) {
    buffer = stream.readStringUntil('\n');
    if (buffer.length() > 0) {
      char *cstr = (char *)calloc(buffer.length()+1, sizeof(*cstr));
      buffer.toCharArray(cstr, buffer.length()+1);
      *minutes += WorkLogger::parseMinutes(cstr);
      free(cstr);
    }
  }
  file.close();
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
