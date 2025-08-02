#include "ui_helpers.h"

void _ui_bar_set_property(lv_obj_t * target, int id, int val)
{
    if(id == _UI_BAR_PROPERTY_VALUE_WITH_ANIM) lv_bar_set_value(target, val, LV_ANIM_ON);
    if(id == _UI_BAR_PROPERTY_VALUE) lv_bar_set_value(target, val, LV_ANIM_OFF);
}

void _ui_basic_set_property(lv_obj_t * target, int id, int val)
{
    if(id == _UI_BASIC_PROPERTY_POSITION_X) lv_obj_set_x(target, val);
    if(id == _UI_BASIC_PROPERTY_POSITION_Y) lv_obj_set_y(target, val);
    if(id == _UI_BASIC_PROPERTY_WIDTH) lv_obj_set_width(target, val);
    if(id == _UI_BASIC_PROPERTY_HEIGHT) lv_obj_set_height(target, val);
}

void _ui_arc_set_text_value(lv_obj_t * trg, lv_obj_t * src, const char * prefix, const char * postfix)
{
    char buf[_UI_TEMPORARY_STRING_BUFFER_SIZE];
    lv_snprintf(buf, sizeof(buf), "%s%d%s", prefix, (int)lv_arc_get_value(src), postfix);
    lv_label_set_text(trg, buf);
}

void _ui_screen_change(lv_obj_t ** target, lv_scr_load_anim_t fademode, int spd, int delay, void (*target_init)(void))
{
    if(*target == NULL)
        target_init();
    lv_scr_load_anim(*target, fademode, spd, delay, false);
}

void _ui_screen_delete(lv_obj_t ** target)
{
    if(*target == NULL) {
        lv_obj_del(*target);
        target = NULL;
    }
}

void _ui_flag_modify(lv_obj_t * target, int32_t flag, int value)
{
    if(value == _UI_MODIFY_FLAG_TOGGLE) {
        if(lv_obj_has_flag(target, flag)) lv_obj_clear_flag(target, flag);
        else lv_obj_add_flag(target, flag);
    }
    else if(value == _UI_MODIFY_FLAG_ADD) lv_obj_add_flag(target, flag);
    else lv_obj_clear_flag(target, flag);
}

void _ui_state_modify(lv_obj_t * target, int32_t state, int value)
{
    if(value == _UI_MODIFY_STATE_TOGGLE) {
        if(lv_obj_has_state(target, state)) lv_obj_clear_state(target, state);
        else lv_obj_add_state(target, state);
    }
    else if(value == _UI_MODIFY_STATE_ADD) lv_obj_add_state(target, state);
    else lv_obj_clear_state(target, state);
}

void _ui_opacity_set(lv_obj_t * target, int val)
{
    lv_obj_set_style_opa(target, val, 0);
}

void scr_unloaded_delete_cb(lv_event_t * e)
{
    lv_obj_t ** var = lv_event_get_user_data(e);
    lv_obj_del(*var);
    (*var) = NULL;
}

void get_component_child_event_cb(lv_event_t * e)
{
    lv_obj_t ** c = lv_event_get_user_data(e);
    ui_comp_get_child_t * info = lv_event_get_param(e);
    info->child = c[info->child_idx];
}

void del_component_child_event_cb(lv_event_t * e)
{
    lv_obj_t ** c = lv_event_get_user_data(e);
    lv_mem_free(c);
}

uint32_t LV_EVENT_GET_COMP_CHILD;

typedef struct {
    uint32_t child_idx;
    lv_obj_t * child;
} ui_comp_get_child_t;

lv_obj_t * ui_comp_get_child(lv_obj_t * comp, uint32_t child_idx)
{
    ui_comp_get_child_t info;
    info.child = NULL;
    info.child_idx = child_idx;
    lv_event_send(comp, LV_EVENT_GET_COMP_CHILD, &info);
    return info.child;
}