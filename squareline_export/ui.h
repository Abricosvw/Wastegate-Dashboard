/**
 * SquareLine Studio UI Header for ECU Dashboard
 * Generated from web application components
 */

#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include "ecu_data_structures.h"

// Screen declarations
extern lv_obj_t *ui_MainScreen;
extern lv_obj_t *ui_SettingsScreen;

// Main screen components
extern lv_obj_t *ui_HeaderPanel;
extern lv_obj_t *ui_TitleLabel;
extern lv_obj_t *ui_ConnectionStatus;
extern lv_obj_t *ui_GaugeGrid;
extern lv_obj_t *ui_StatusBanner;
extern lv_obj_t *ui_ControlPanel;

// Gauge objects
extern lv_obj_t *ui_MapPressureGauge;
extern lv_obj_t *ui_WastegateGauge;
extern lv_obj_t *ui_TpsGauge;
extern lv_obj_t *ui_RpmGauge;
extern lv_obj_t *ui_TargetBoostGauge;
extern lv_obj_t *ui_TcuStatusPanel;

// Settings screen components
extern lv_obj_t *ui_SettingsPanel;
extern lv_obj_t *ui_SizeSlider;
extern lv_obj_t *ui_ColumnsSlider;
extern lv_obj_t *ui_StyleDropdown;

// Value labels
extern lv_obj_t *ui_MapValueLabel;
extern lv_obj_t *ui_WastegateValueLabel;
extern lv_obj_t *ui_TpsValueLabel;
extern lv_obj_t *ui_RpmValueLabel;
extern lv_obj_t *ui_TargetValueLabel;

// Screen initialization functions
void ui_MainScreen_screen_init(void);
void ui_SettingsScreen_screen_init(void);

// Component creation functions
void ui_create_map_pressure_gauge(void);
void ui_create_wastegate_gauge(void);
void ui_create_tps_gauge(void);
void ui_create_rpm_gauge(void);
void ui_create_target_boost_gauge(void);
void ui_create_tcu_status_panel(void);

// Event handler functions
void ui_events_init(void);
void ui_update_timer_callback(lv_timer_t *timer);
void ui_settings_button_event_handler(lv_event_t *e);
void ui_back_button_event_handler(lv_event_t *e);
void ui_size_slider_event_handler(lv_event_t *e);
void ui_columns_slider_event_handler(lv_event_t *e);

// Data update functions
void ui_set_ecu_data(const ecu_data_t *data);
void ui_set_connection_status(bool connected, const char *message);
void ui_set_display_settings(const display_settings_t *settings);
display_settings_t* ui_get_display_settings(void);

// Utility functions
void ui_update_gauges(void);
void ui_update_connection_status(void);
void ui_animate_gauge_transition(lv_obj_t *gauge, int32_t new_value);
void ui_apply_display_settings(void);

// Cleanup function
void ui_events_cleanup(void);

// Main initialization function
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // UI_H