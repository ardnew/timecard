#include "Navigator.h"

Navigator::Navigator(void):
  _selector(),
  _controller(),
  _tabView(nullptr),
  _selectTab(nullptr),
  _controlTab(nullptr)
{
  ; // empty
}

void Navigator::initLayout(lv_obj_t *parent)
{
  const lv_coord_t tabViewPosY = STATUS_BAR_HEIGHT;

  static lv_style_t tabStyle;
  lv_style_init(&tabStyle);

  lv_style_set_text_font(&tabStyle, LV_STATE_DEFAULT, &lv_font_montserrat_14);
  lv_style_set_pad_top(&tabStyle, LV_STATE_DEFAULT, 6);
  lv_style_set_pad_bottom(&tabStyle, LV_STATE_DEFAULT, 6);
  lv_style_set_pad_left(&tabStyle, LV_STATE_DEFAULT, 0);
  lv_style_set_pad_right(&tabStyle, LV_STATE_DEFAULT, 0);

  _tabView = lv_tabview_create(parent, NULL);
  lv_obj_add_style(_tabView, LV_TABVIEW_PART_TAB_BG, &tabStyle);
  lv_obj_add_style(_tabView, LV_TABVIEW_PART_TAB_BTN, &tabStyle);
  lv_obj_set_size(_tabView, NAVIGATOR_WIDTH, NAVIGATOR_HEIGHT);
  lv_obj_set_pos(_tabView, 0, tabViewPosY);
  lv_tabview_set_btns_pos(_tabView, LV_TABVIEW_TAB_POS_BOTTOM);

  _selectTab = lv_tabview_add_tab(_tabView, LV_SYMBOL_LIST " Activity");
  _controlTab = lv_tabview_add_tab(_tabView, "Clock");

  _selector.initLayout(_selectTab);
  _controller.initLayout(_controlTab);
}

void Navigator::update(void)
{
  _selector.update();
  _controller.update();
}
