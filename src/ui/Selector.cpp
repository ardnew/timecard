#include "Selector.h"

Selector::Selector(void):
  _proList(nullptr),
  _actRoll(nullptr)
{
  ; // empty
}

void Selector::initLayout(lv_obj_t *parent)
{
  const lv_coord_t listWidth     = NAVIGATOR_WIDTH - (2 * NAVIGATOR_H_PAD);
  const lv_coord_t rollerWidth   = listWidth;

  static lv_style_t listStyle;
  lv_style_init(&listStyle);
  lv_style_set_text_font(&listStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12);

  _proList = lv_dropdown_create(parent, NULL);
  lv_obj_add_style(_proList, LV_OBJ_PART_MAIN, &listStyle);
  lv_obj_add_style(_proList, LV_DROPDOWN_PART_LIST, &listStyle);
  lv_obj_set_width(_proList, listWidth);
  lv_obj_align(_proList, parent, LV_ALIGN_IN_TOP_MID, 0, NAVIGATOR_V_PAD);

  static lv_style_t rollerStyle;
  lv_style_init(&rollerStyle);
  lv_style_set_text_font(&rollerStyle, LV_STATE_DEFAULT, &lv_font_montserrat_12);

  _actRoll = lv_roller_create(parent, NULL);
  lv_roller_set_align(_actRoll, LV_LABEL_ALIGN_LEFT);
  lv_obj_add_style(_actRoll, LV_OBJ_PART_MAIN, &rollerStyle);
  lv_roller_set_auto_fit(_actRoll, false);
  lv_roller_set_visible_row_count(_actRoll, ROLLER_ACTIVITIES);
  lv_obj_set_width(_actRoll, rollerWidth);
  lv_obj_align(_actRoll, _proList, LV_ALIGN_OUT_BOTTOM_MID, 0, NAVIGATOR_V_PAD);
  // lv_obj_set_event_cb(_actRoll, event_handler);
}

void Selector::update(void)
{
}

void Selector::setProjects(const char *projects)
{
  lv_dropdown_clear_options(_proList);
  if (nullptr != projects) {
    lv_dropdown_set_options(_proList, projects);
  }
}

void Selector::setActivities(const char *activities)
{
  if (nullptr != activities) {
    lv_roller_set_options(_actRoll, activities, LV_ROLLER_MODE_NORMAL);
  } else {
    lv_roller_set_options(_actRoll, "No activities!", LV_ROLLER_MODE_NORMAL);
  }
}
