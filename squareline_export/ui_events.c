/**
 * SquareLine Studio UI Event Handlers for ECU Dashboard
 * Generated from web application event handling
 */

#include "ui.h"
#include "ecu_data_structures.h"

// Global variables for current data
static ecu_data_t current_ecu_data = {0};
static display_settings_t current_display_settings = {0};
static connection_status_t current_connection_status = {0};

// Timer for periodic updates
static lv_timer_t *update_timer;

// Function prototypes
void ui_update_gauges(void);
void ui_update_connection_status(void);
void ui_handle_gauge_threshold_change(float value, uint32_t warning, uint32_t danger);
void ui_animate_gauge_transition(lv_obj_t *gauge, int32_t new_value);

// Initialize UI events and timers
void ui_events_init(void)
{
    // Initialize default display settings
    current_display_settings.gauge_size = 1;  // Medium
    current_display_settings.columns = 3;
    current_display_settings.show_titles = true;
    current_display_settings.show_values = true;
    current_display_settings.show_targets = true;
    current_display_settings.compact_mode = false;
    current_display_settings.gauge_style = 0;  // Circular
    current_display_settings.visible_gauges = 0x3F;  // All visible

    // Create update timer - 50ms (20Hz)
    update_timer = lv_timer_create(ui_update_timer_callback, 50, NULL);
}

// Main update timer callback
void ui_update_timer_callback(lv_timer_t *timer)
{
    (void)timer;
    
    // Update all gauges with current data
    ui_update_gauges();
    
    // Update connection status
    ui_update_connection_status();
}

// Update all gauge values and colors
void ui_update_gauges(void)
{
    // Update MAP pressure gauge
    if (current_display_settings.visible_gauges & GAUGE_MAP_VISIBLE) {
        ui_animate_gauge_transition(ui_MapPressureGauge, (int32_t)current_ecu_data.map_pressure);
        
        // Update value label
        char value_str[16];
        snprintf(value_str, sizeof(value_str), "%.1f", current_ecu_data.map_pressure);
        lv_label_set_text(ui_MapValueLabel, value_str);
        
        // Check thresholds and update color
        if (current_ecu_data.map_pressure >= 245.0f) {
            lv_obj_set_style_arc_color(ui_MapPressureGauge, lv_color_hex(COLOR_DANGER), LV_PART_INDICATOR);
            lv_obj_set_style_text_color(ui_MapValueLabel, lv_color_hex(COLOR_DANGER), LV_PART_MAIN);
        } else if (current_ecu_data.map_pressure >= 230.0f) {
            lv_obj_set_style_arc_color(ui_MapPressureGauge, lv_color_hex(COLOR_WARNING), LV_PART_INDICATOR);
            lv_obj_set_style_text_color(ui_MapValueLabel, lv_color_hex(COLOR_WARNING), LV_PART_MAIN);
        } else {
            lv_obj_set_style_arc_color(ui_MapPressureGauge, lv_color_hex(COLOR_ACCENT), LV_PART_INDICATOR);
            lv_obj_set_style_text_color(ui_MapValueLabel, lv_color_hex(COLOR_ACCENT), LV_PART_MAIN);
        }
    }

    // Update Wastegate gauge
    if (current_display_settings.visible_gauges & GAUGE_WASTEGATE_VISIBLE) {
        ui_animate_gauge_transition(ui_WastegateGauge, (int32_t)current_ecu_data.wastegate_position);
        
        char value_str[16];
        snprintf(value_str, sizeof(value_str), "%.1f", current_ecu_data.wastegate_position);
        lv_label_set_text(ui_WastegateValueLabel, value_str);
    }

    // Update TPS gauge
    if (current_display_settings.visible_gauges & GAUGE_TPS_VISIBLE) {
        ui_animate_gauge_transition(ui_TpsGauge, (int32_t)current_ecu_data.tps_position);
        
        char value_str[16];
        snprintf(value_str, sizeof(value_str), "%.1f", current_ecu_data.tps_position);
        lv_label_set_text(ui_TpsValueLabel, value_str);
    }

    // Update RPM gauge
    if (current_display_settings.visible_gauges & GAUGE_RPM_VISIBLE) {
        ui_animate_gauge_transition(ui_RpmGauge, (int32_t)current_ecu_data.engine_rpm);
        
        char value_str[16];
        snprintf(value_str, sizeof(value_str), "%.0f", current_ecu_data.engine_rpm);
        lv_label_set_text(ui_RpmValueLabel, value_str);
        
        // RPM threshold colors
        if (current_ecu_data.engine_rpm >= 6500.0f) {
            lv_obj_set_style_arc_color(ui_RpmGauge, lv_color_hex(COLOR_DANGER), LV_PART_INDICATOR);
            lv_obj_set_style_text_color(ui_RpmValueLabel, lv_color_hex(COLOR_DANGER), LV_PART_MAIN);
        } else if (current_ecu_data.engine_rpm >= 6000.0f) {
            lv_obj_set_style_arc_color(ui_RpmGauge, lv_color_hex(COLOR_WARNING), LV_PART_INDICATOR);
            lv_obj_set_style_text_color(ui_RpmValueLabel, lv_color_hex(COLOR_WARNING), LV_PART_MAIN);
        } else {
            lv_obj_set_style_arc_color(ui_RpmGauge, lv_color_hex(COLOR_WARNING), LV_PART_INDICATOR);
            lv_obj_set_style_text_color(ui_RpmValueLabel, lv_color_hex(COLOR_WARNING), LV_PART_MAIN);
        }
    }

    // Update Target Boost gauge
    if (current_display_settings.visible_gauges & GAUGE_TARGET_VISIBLE) {
        ui_animate_gauge_transition(ui_TargetBoostGauge, (int32_t)current_ecu_data.target_boost);
        
        char value_str[16];
        snprintf(value_str, sizeof(value_str), "%.1f", current_ecu_data.target_boost);
        lv_label_set_text(ui_TargetValueLabel, value_str);
    }

    // Update TCU Status
    if (current_display_settings.visible_gauges & GAUGE_TCU_VISIBLE) {
        if (current_ecu_data.tcu_limp_mode) {
            lv_obj_set_style_border_color(ui_TcuStatusPanel, lv_color_hex(COLOR_DANGER), LV_PART_MAIN);
        } else if (current_ecu_data.tcu_protection_active) {
            lv_obj_set_style_border_color(ui_TcuStatusPanel, lv_color_hex(COLOR_WARNING), LV_PART_MAIN);
        } else {
            lv_obj_set_style_border_color(ui_TcuStatusPanel, lv_color_hex(COLOR_SUCCESS), LV_PART_MAIN);
        }
    }
}

// Smooth gauge animation
void ui_animate_gauge_transition(lv_obj_t *gauge, int32_t new_value)
{
    int32_t current_value = lv_arc_get_value(gauge);
    
    // Create animation
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, gauge);
    lv_anim_set_values(&anim, current_value, new_value);
    lv_anim_set_time(&anim, 200);  // 200ms animation
    lv_anim_set_exec_cb(&anim, (lv_anim_exec_xcb_t)lv_arc_set_value);
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);
    lv_anim_start(&anim);
}

// Update connection status display
void ui_update_connection_status(void)
{
    if (current_connection_status.connected) {
        lv_label_set_text(ui_ConnectionStatus, "Connected");
        lv_obj_set_style_text_color(ui_ConnectionStatus, lv_color_hex(COLOR_SUCCESS), LV_PART_MAIN);
    } else {
        lv_label_set_text(ui_ConnectionStatus, "Disconnected");
        lv_obj_set_style_text_color(ui_ConnectionStatus, lv_color_hex(COLOR_DANGER), LV_PART_MAIN);
    }
}

// Event handler for settings button
void ui_settings_button_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_CLICKED) {
        lv_scr_load_anim(ui_SettingsScreen, LV_SCR_LOAD_ANIM_SLIDE_LEFT, 300, 0, false);
    }
}

// Event handler for back button in settings
void ui_back_button_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    
    if (code == LV_EVENT_CLICKED) {
        lv_scr_load_anim(ui_MainScreen, LV_SCR_LOAD_ANIM_SLIDE_RIGHT, 300, 0, false);
    }
}

// Event handler for size slider
void ui_size_slider_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *slider = lv_event_get_target(e);
    
    if (code == LV_EVENT_VALUE_CHANGED) {
        current_display_settings.gauge_size = lv_slider_get_value(slider);
        ui_apply_display_settings();
    }
}

// Event handler for columns slider
void ui_columns_slider_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *slider = lv_event_get_target(e);
    
    if (code == LV_EVENT_VALUE_CHANGED) {
        current_display_settings.columns = lv_slider_get_value(slider);
        ui_apply_display_settings();
    }
}

// Apply display settings to UI
void ui_apply_display_settings(void)
{
    // Update gauge sizes
    int gauge_size = 120;  // Small
    switch (current_display_settings.gauge_size) {
        case 0: gauge_size = 120; break;  // Small
        case 1: gauge_size = 180; break;  // Medium
        case 2: gauge_size = 240; break;  // Large
        case 3: gauge_size = 300; break;  // XLarge
    }
    
    // Apply size to all gauges
    lv_obj_set_size(ui_MapPressureGauge, gauge_size, gauge_size);
    lv_obj_set_size(ui_WastegateGauge, gauge_size, gauge_size);
    lv_obj_set_size(ui_TpsGauge, gauge_size, gauge_size);
    lv_obj_set_size(ui_RpmGauge, gauge_size, gauge_size);
    lv_obj_set_size(ui_TargetBoostGauge, gauge_size, gauge_size);
    lv_obj_set_size(ui_TcuStatusPanel, gauge_size, gauge_size);
    
    // Update grid layout based on columns
    lv_obj_set_flex_flow(ui_GaugeGrid, LV_FLEX_FLOW_ROW_WRAP);
    
    // Hide/show gauges based on visibility settings
    lv_obj_add_flag(ui_MapPressureGauge, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_WastegateGauge, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_TpsGauge, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_RpmGauge, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_TargetBoostGauge, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_TcuStatusPanel, LV_OBJ_FLAG_HIDDEN);
    
    if (current_display_settings.visible_gauges & GAUGE_MAP_VISIBLE)
        lv_obj_clear_flag(ui_MapPressureGauge, LV_OBJ_FLAG_HIDDEN);
    if (current_display_settings.visible_gauges & GAUGE_WASTEGATE_VISIBLE)
        lv_obj_clear_flag(ui_WastegateGauge, LV_OBJ_FLAG_HIDDEN);
    if (current_display_settings.visible_gauges & GAUGE_TPS_VISIBLE)
        lv_obj_clear_flag(ui_TpsGauge, LV_OBJ_FLAG_HIDDEN);
    if (current_display_settings.visible_gauges & GAUGE_RPM_VISIBLE)
        lv_obj_clear_flag(ui_RpmGauge, LV_OBJ_FLAG_HIDDEN);
    if (current_display_settings.visible_gauges & GAUGE_TARGET_VISIBLE)
        lv_obj_clear_flag(ui_TargetBoostGauge, LV_OBJ_FLAG_HIDDEN);
    if (current_display_settings.visible_gauges & GAUGE_TCU_VISIBLE)
        lv_obj_clear_flag(ui_TcuStatusPanel, LV_OBJ_FLAG_HIDDEN);
}

// Public API functions for updating data
void ui_set_ecu_data(const ecu_data_t *data)
{
    if (data) {
        current_ecu_data = *data;
    }
}

void ui_set_connection_status(bool connected, const char *message)
{
    current_connection_status.connected = connected;
    current_connection_status.message = message;
}

void ui_set_display_settings(const display_settings_t *settings)
{
    if (settings) {
        current_display_settings = *settings;
        ui_apply_display_settings();
    }
}

// Get current settings
display_settings_t* ui_get_display_settings(void)
{
    return &current_display_settings;
}

// Cleanup function
void ui_events_cleanup(void)
{
    if (update_timer) {
        lv_timer_del(update_timer);
        update_timer = NULL;
    }
}