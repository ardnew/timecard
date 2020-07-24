#ifndef __STATUS_BAR_H__
#define __STATUS_BAR_H__

#include "../../timecard.h"

#include "Dimensions.h"

class StatusBar
{
protected:
  enum class WiFiSignal { None, Pct0, Pct25, Pct50, Pct75, Pct100 };

  lv_obj_t *_labelTime;
  lv_obj_t *_imageWiFi;

public:
  StatusBar(void);

  void initLayout(lv_obj_t *parent);
  void updateTime(char *dateTime);
  void updateWiFi(bool isConnected, int signalQuality);
};

#endif // __STATUS_BAR_H__
