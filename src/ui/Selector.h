#ifndef __SELECTOR_H__
#define __SELECTOR_H__

#include "../../timecard.h"

#include "Dimensions.h"

class Selector
{
protected:
  lv_obj_t *_proList;
  lv_obj_t *_actRoll;
public:
  Selector(void);
  void initLayout(lv_obj_t *parent);
  void update(void);
  void setProjects(const char *projects);
  void setActivities(const char *activities);
};

#endif // __SELECTOR_H__
