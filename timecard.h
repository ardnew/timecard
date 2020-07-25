#ifndef __TIMECARD_H__
#define __TIMECARD_H__

#include <Arduino.h>
#include <SPI.h>

#include <ArduinoJson.h>
#include <StreamUtils.h>

#include <lvgl.h>
#include <Adafruit_LvGL_Glue.h>

#include <ezTime.h>

// PyPortal-specific includes
#include <SdFat.h>
#include <WiFiNINA.h>
#include <Adafruit_ILI9341.h>
#include <TouchScreen.h>

#define FILE_WIFI_JSON         "wifi.json"
#define FILE_PROJECTS_JSON     "projects.json"

#define CLOCK_TIMEZONE         "America/Chicago"

#define MAX_HOURS_PER_DAY      12
#define REG_HOURS_PER_DAY      8

#define CPU_SLEEP_DELAY        5 // milliseconds

#define PRINTF_DEBUG_MAX_LEN   256 // undef or 0 to disable debug printing

#if PRINTF_DEBUG_MAX_LEN
  #define PRINTF_DEBUG
  #define WAIT_FOR_SERIAL(timeout, baud) \
      while (!Serial && (millis() < (timeout))) \
        { continue; } Serial.begin(baud);
#else
  #define WAIT_FOR_SERIAL(timeout, baud) \
      /* debug code omitted */
#endif

typedef unsigned long timespan_t; // storage for arduino's millis()

#define sizeof_array(x) (sizeof(x) / sizeof(*(x)))

typedef enum
{
  ilNONE = -1,
  ilInfo,  // = 0
  ilWarn,  // = 1
  ilError, // = 2
  ilCOUNT  // = 3
}
info_level_t;

void print(info_level_t level, const char *fmt, ...);

#define infof(fmt, ...) print(ilInfo, fmt, __VA_ARGS__)
#define warnf(fmt, ...) print(ilWarn, fmt, __VA_ARGS__)
#define errf(fmt, ...)  print(ilError, fmt, __VA_ARGS__)

#endif // __TIMECARD_H__
