#include "StatusBar.h"

LV_IMG_DECLARE(wifi_signal_0);
LV_IMG_DECLARE(wifi_signal_25);
LV_IMG_DECLARE(wifi_signal_50);
LV_IMG_DECLARE(wifi_signal_75);
LV_IMG_DECLARE(wifi_signal_100);

StatusBar::StatusBar(void):
  _labelTime(nullptr),
  _imageWiFi(nullptr)
{
  ; // empty
}

void StatusBar::initLayout(lv_obj_t *parent)
{
  const lv_coord_t imageWiFiPosX =
    STATUS_BAR_WIDTH - (WIFI_IMAGE_WIDTH + STATUS_BAR_H_PAD);
  const lv_coord_t labelTimeWidth = imageWiFiPosX;

  _imageWiFi = lv_img_create(parent, NULL);
  lv_obj_set_pos(_imageWiFi, imageWiFiPosX, STATUS_BAR_V_PAD);

  _labelTime = lv_label_create(parent, NULL);
  lv_obj_set_pos(_labelTime, STATUS_BAR_H_PAD, STATUS_BAR_V_PAD);
  lv_obj_set_width(_labelTime, labelTimeWidth);
}

void StatusBar::updateTime(char *dateTime)
{
  lv_label_set_text(_labelTime, dateTime);
}

void StatusBar::updateWiFi(bool isConnected, int signalQuality)
{
  static WiFiSignal lastSignal = WiFiSignal::None;

  if (isConnected) {
    if (signalQuality > 75) {
      if (WiFiSignal::Pct100 != lastSignal) {
        lv_img_set_src(_imageWiFi, &wifi_signal_100);
        lastSignal = WiFiSignal::Pct100;
      }
    } else if (signalQuality > 50) {
      if (WiFiSignal::Pct75 != lastSignal) {
        lv_img_set_src(_imageWiFi, &wifi_signal_75);
        lastSignal = WiFiSignal::Pct75;
      }
    } else if (signalQuality > 25) {
      if (WiFiSignal::Pct50 != lastSignal) {
        lv_img_set_src(_imageWiFi, &wifi_signal_50);
        lastSignal = WiFiSignal::Pct50;
      }
    } else {
      if (WiFiSignal::Pct25 != lastSignal) {
        lv_img_set_src(_imageWiFi, &wifi_signal_25);
        lastSignal = WiFiSignal::Pct25;
      }
    }
  } else {
    if (WiFiSignal::Pct0 != lastSignal) {
      lv_img_set_src(_imageWiFi, &wifi_signal_0);
      lastSignal = WiFiSignal::Pct0;
    }
  }
}
