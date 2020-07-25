#ifndef __DIMENSIONS_H__
#define __DIMENSIONS_H__

#define SCREEN_WIDTH                 240
#define SCREEN_HEIGHT                320

#define WIFI_IMAGE_WIDTH             22
#define WIFI_IMAGE_HEIGHT            16

#define STATUS_BAR_H_PAD             8
#define STATUS_BAR_V_PAD             6

#define STATUS_BAR_WIDTH             (SCREEN_WIDTH)
#define STATUS_BAR_HEIGHT            ((WIFI_IMAGE_HEIGHT) + (2 * (STATUS_BAR_V_PAD)))

#define NAVIGATOR_H_PAD              12
#define NAVIGATOR_V_PAD              10

#define NAVIGATOR_WIDTH              (SCREEN_WIDTH)
#define NAVIGATOR_HEIGHT             ((SCREEN_HEIGHT) - (STATUS_BAR_HEIGHT))

#define ROLLER_ACTIVITIES            5

#define TIME_GAUGE_WIDTH             190
#define TIME_GAUGE_HEIGHT            190

#define TIME_GAUGE_H_ALIGN           0
#define TIME_GAUGE_V_ALIGN           10

#define TIME_GAUGE_ANGLE             220
#define TIME_GAUGE_HOURS_PER_LABEL   2
#define TIME_GAUGE_MINOR_SECTIONS    4 // 2 hours (per label) / 4 = 30 mins per minor tick
#define TIME_GAUGE_LABEL_COUNT       MAX_HOURS_PER_DAY / TIME_GAUGE_HOURS_PER_LABEL + 1
#define TIME_GAUGE_TICK_COUNT        TIME_GAUGE_MINOR_SECTIONS * (TIME_GAUGE_LABEL_COUNT - 1) + 1

#define OVERTIME_H_ALIGN             -16
#define OVERTIME_V_ALIGN             8
#define OVERTIME_LABEL_SPACING       6

#define RESTART_TOGGLE_V_ALIGN       50

#endif // __DIMENSIONS_H__
