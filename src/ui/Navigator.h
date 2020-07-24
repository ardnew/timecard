#ifndef __NAVIGATOR_H__
#define __NAVIGATOR_H__

#include "../../timecard.h"

#include "Dimensions.h"
#include "Selector.h"
#include "Controller.h"

class Navigator
{
protected:
  Selector _selector;
  Controller _controller;
  lv_obj_t *_tabView;
  lv_obj_t *_selectTab;
  lv_obj_t *_controlTab;
public:
  Navigator(void);
  void initLayout(lv_obj_t *parent);
  void update(void);
  Selector *selector(void) { return &_selector; }
  Controller *controller(void) { return &_controller; }
};

#endif // __NAVIGATOR_H__
