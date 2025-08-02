#ifndef _UI_HELPERS_H
#define _UI_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ui.h"

#define _UI_TEMPORARY_STRING_BUFFER_SIZE 32
#define _UI_BAR_PROPERTY_VALUE 0
#define _UI_BAR_PROPERTY_VALUE_WITH_ANIM 1
void _ui_bar_set_property(lv_obj_t * target, int id, int val);

#define _UI_BASIC_PROPERTY_POSITION_X 0
#define _UI_BASIC_PROPERTY_POSITION_Y 1
#define _UI_BASIC_PROPERTY_WIDTH 2
#define _UI_BASIC_PROPERTY_HEIGHT 3
void _ui_basic_set_property(lv_obj_t * target, int id, int val);

void _ui_arc_set_text_value(lv_obj_t * trg, lv_obj_t * src, const char * prefix, const char * postfix);

void _ui_screen_change(lv_obj_t ** target, lv_scr_load_anim_t fademode, int spd, int delay, void (*target_init)(void));

void _ui_screen_delete(lv_obj_t ** target);

#define _UI_MODIFY_FLAG_ADD 0
#define _UI_MODIFY_FLAG_REMOVE 1
#define _UI_MODIFY_FLAG_TOGGLE 2
void _ui_flag_modify(lv_obj_t * target, int32_t flag, int value);

#define _UI_MODIFY_STATE_ADD 0
#define _UI_MODIFY_STATE_REMOVE 1
#define _UI_MODIFY_STATE_TOGGLE 2
void _ui_state_modify(lv_obj_t * target, int32_t state, int value);

void _ui_opacity_set(lv_obj_t * target, int val);

void scr_unloaded_delete_cb(lv_event_t * e);

void get_component_child_event_cb(lv_event_t * e);
void del_component_child_event_cb(lv_event_t * e);

typedef struct {
    uint32_t child_idx;
    lv_obj_t * child;
} ui_comp_get_child_t;

lv_obj_t * ui_comp_get_child(lv_obj_t * comp, uint32_t child_idx);
extern uint32_t LV_EVENT_GET_COMP_CHILD;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif