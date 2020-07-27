#ifndef __PYPORTAL_H__
#define __PYPORTAL_H__

#include "../Board.h"
#include "../../timecard.h"

// use 8-bit parallel interface (not SPI)
#define TFT_IF_PARALLEL    1
#define TFT_ROTATION       0

// TFT LCD
#define PIN_TFT_D0        34
#define PIN_TFT_WR        26
#define PIN_TFT_DC        10
#define PIN_TFT_CS        11
#define PIN_TFT_RST       24
#define PIN_TFT_RD         9
#define PIN_TFT_BKL       25

// Touchscreen
#define PIN_TS_YP         A4
#define PIN_TS_XP         A5
#define PIN_TS_YM         A6
#define PIN_TS_XM         A7

// SD
#define PIN_SD_CS         32
#define PARSE_BUF_SZ     512 // bytes

// ESP32
#define SPI_ESP32        SPI
#define UART_ESP32   Serial1
#define PIN_ESP32_ACK      5
#define PIN_ESP32_GPIO     6
#define PIN_ESP32_RST      7
#define PIN_ESP32_CS       8

class PyPortal: public Board
{
private:
  uint8_t              _statusWiFi;

protected:
  SdFat               *_sd;
  WiFiClass           *_wifi;
  Adafruit_ILI9341    *_tft;
  TouchScreen         *_ts;
  Adafruit_LvGL_Glue  *_glue;

  bool initSd();
  bool initWifi();
  bool initLcd();

  bool bindGfx();

public:
  PyPortal(void);

  Board::InitError begin(void);

  bool loadWiFiSettings(const char *filepath);
  bool connectToWiFi(void);
  bool isConnectedWiFi(void);
  int signalQualityWiFi(void);

  bool loadProjectSettings(const char *filepath);

  bool assertWorkLog(char **filepath, const TimeStamp &timeStamp);
  bool appendWorkLogEntry(char *filepath, char *entry);
  bool parseWorkLogMinutes(uint32_t *minutes, char *filepath);
};

#endif // __PYPORTAL_H__
