#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "../../timecard.h"

#include "Dimensions.h"

class Controller
{
public:
  Controller(void);
  void initLayout(lv_obj_t *parent);
  void update(void);
};

#endif // __CONTROLLER_H__
