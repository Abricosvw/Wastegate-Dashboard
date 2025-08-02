/**
 * Complete UI implementation for SquareLine Studio
 * ECU Dashboard with 6 automotive gauges
 */

#include "ui.h"
#include "ecu_data_structures.h"

// Screen objects
lv_obj_t *ui_MainScreen;
lv_obj_t *ui_HeaderPanel;
lv_obj_t *ui_TitleLabel;
lv_obj_t *ui_ConnectionStatus;
lv_obj_t *ui_GaugeGrid;

// Gauge objects
lv_obj_t *ui_MapGauge;
lv_obj_t *ui_WastegateGauge;
lv_obj_t *ui_TpsGauge;
lv_obj_t *ui_RpmGauge;
lv_obj_t *ui_TargetGauge;
lv_obj_t *ui_TcuPanel;

// Value labels
lv_obj_t *ui_MapValueLabel;
lv_obj_t *ui_WastegateValueLabel;
lv_obj_t *ui_TpsValueLabel;
lv_obj_t *ui_RpmValueLabel;
lv_obj_t *ui_TargetValueLabel;
lv_obj_t *ui_TcuStatusLabel;

// Color definitions (copy these to SquareLine Studio)
#define COLOR_BACKGROUND    0x0A0A0A
#define COLOR_CARD          0x1A1A1A
#define COLOR_TEXT_PRIMARY  0xFFFFFF
#define COLOR_TEXT_SECONDARY 0x888888
#define COLOR_BORDER        0x333333
#define COLOR_MAP           0x00D4FF  // Blue
#define COLOR_WASTEGATE     0x00FF88  // Green
#define COLOR_TPS           0xFFD700  // Yellow
#define COLOR_RPM           0xFF6B35  // Orange
#define COLOR_TARGET        0xFFD700  // Yellow
#define COLOR_SUCCESS       0x00FF88
#define COLOR_WARNING       0xFF6B35
#define COLOR_DANGER        0xFF3366

void ui_init(void)
{
    ui_MainScreen_screen_init();
    lv_disp_load_scr(ui_MainScreen);
}

void ui_MainScreen_screen_init(void)
{
    // Create main screen
    ui_MainScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_MainScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_MainScreen, lv_color_hex(COLOR_BACKGROUND), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Header panel
    ui_HeaderPanel = lv_obj_create(ui_MainScreen);
    lv_obj_set_width(ui_HeaderPanel, lv_pct(100));
    lv_obj_set_height(ui_HeaderPanel, 60);
    lv_obj_set_align(ui_HeaderPanel, LV_ALIGN_TOP_MID);
    lv_obj_set_style_bg_color(ui_HeaderPanel, lv_color_hex(COLOR_CARD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_HeaderPanel, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_HeaderPanel, lv_color_hex(COLOR_BORDER), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Title label
    ui_TitleLabel = lv_label_create(ui_HeaderPanel);
    lv_obj_set_align(ui_TitleLabel, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_TitleLabel, 20);
    lv_label_set_text(ui_TitleLabel, "Turbo Control Dashboard");
    lv_obj_set_style_text_color(ui_TitleLabel, lv_color_hex(COLOR_TEXT_PRIMARY), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Connection status
    ui_ConnectionStatus = lv_label_create(ui_HeaderPanel);
    lv_obj_set_align(ui_ConnectionStatus, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(ui_ConnectionStatus, -20);
    lv_label_set_text(ui_ConnectionStatus, "Disconnected");
    lv_obj_set_style_text_color(ui_ConnectionStatus, lv_color_hex(COLOR_DANGER), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Create all gauges
    ui_create_map_gauge();
    ui_create_wastegate_gauge();
    ui_create_tps_gauge();
    ui_create_rpm_gauge();
    ui_create_target_gauge();
    ui_create_tcu_panel();
}

void ui_create_map_gauge(void)
{
    // MAP Pressure Gauge - Top Left
    ui_MapGauge = lv_arc_create(ui_MainScreen);
    lv_obj_set_width(ui_MapGauge, 200);
    lv_obj_set_height(ui_MapGauge, 200);
    lv_obj_set_x(ui_MapGauge, -260);
    lv_obj_set_y(ui_MapGauge, 90);
    lv_obj_set_align(ui_MapGauge, LV_ALIGN_CENTER);
    
    lv_arc_set_range(ui_MapGauge, 100, 250);
    lv_arc_set_value(ui_MapGauge, 150);
    lv_arc_set_bg_angles(ui_MapGauge, 135, 45);
    lv_obj_set_style_arc_color(ui_MapGauge, lv_color_hex(COLOR_MAP), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_MapGauge, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_MapGauge, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_MapGauge, lv_color_hex(COLOR_BORDER), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(ui_MapGauge, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(ui_MapGauge, LV_OBJ_FLAG_CLICKABLE);

    // MAP Labels
    lv_obj_t *map_title = lv_label_create(ui_MapGauge);
    lv_obj_set_align(map_title, LV_ALIGN_CENTER);
    lv_obj_set_y(map_title, -40);
    lv_label_set_text(map_title, "MAP");
    lv_obj_set_style_text_color(map_title, lv_color_hex(COLOR_TEXT_PRIMARY), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_MapValueLabel = lv_label_create(ui_MapGauge);
    lv_obj_set_align(ui_MapValueLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_MapValueLabel, "150");
    lv_obj_set_style_text_color(ui_MapValueLabel, lv_color_hex(COLOR_MAP), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *map_unit = lv_label_create(ui_MapGauge);
    lv_obj_set_align(map_unit, LV_ALIGN_CENTER);
    lv_obj_set_y(map_unit, 30);
    lv_label_set_text(map_unit, "kPa");
    lv_obj_set_style_text_color(map_unit, lv_color_hex(COLOR_TEXT_SECONDARY), LV_PART_MAIN | LV_STATE_DEFAULT);
}

void ui_create_wastegate_gauge(void)
{
    // Wastegate Gauge - Top Center
    ui_WastegateGauge = lv_arc_create(ui_MainScreen);
    lv_obj_set_width(ui_WastegateGauge, 200);
    lv_obj_set_height(ui_WastegateGauge, 200);
    lv_obj_set_x(ui_WastegateGauge, 0);
    lv_obj_set_y(ui_WastegateGauge, 90);
    lv_obj_set_align(ui_WastegateGauge, LV_ALIGN_CENTER);
    
    lv_arc_set_range(ui_WastegateGauge, 0, 100);
    lv_arc_set_value(ui_WastegateGauge, 45);
    lv_arc_set_bg_angles(ui_WastegateGauge, 135, 45);
    lv_obj_set_style_arc_color(ui_WastegateGauge, lv_color_hex(COLOR_WASTEGATE), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_WastegateGauge, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_WastegateGauge, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WastegateGauge, lv_color_hex(COLOR_BORDER), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(ui_WastegateGauge, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(ui_WastegateGauge, LV_OBJ_FLAG_CLICKABLE);

    // Wastegate Labels
    lv_obj_t *wg_title = lv_label_create(ui_WastegateGauge);
    lv_obj_set_align(wg_title, LV_ALIGN_CENTER);
    lv_obj_set_y(wg_title, -40);
    lv_label_set_text(wg_title, "Wastegate");
    lv_obj_set_style_text_color(wg_title, lv_color_hex(COLOR_TEXT_PRIMARY), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WastegateValueLabel = lv_label_create(ui_WastegateGauge);
    lv_obj_set_align(ui_WastegateValueLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WastegateValueLabel, "45");
    lv_obj_set_style_text_color(ui_WastegateValueLabel, lv_color_hex(COLOR_WASTEGATE), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *wg_unit = lv_label_create(ui_WastegateGauge);
    lv_obj_set_align(wg_unit, LV_ALIGN_CENTER);
    lv_obj_set_y(wg_unit, 30);
    lv_label_set_text(wg_unit, "%");
    lv_obj_set_style_text_color(wg_unit, lv_color_hex(COLOR_TEXT_SECONDARY), LV_PART_MAIN | LV_STATE_DEFAULT);
}

void ui_create_tps_gauge(void)
{
    // TPS Gauge - Top Right
    ui_TpsGauge = lv_arc_create(ui_MainScreen);
    lv_obj_set_width(ui_TpsGauge, 200);
    lv_obj_set_height(ui_TpsGauge, 200);
    lv_obj_set_x(ui_TpsGauge, 260);
    lv_obj_set_y(ui_TpsGauge, 90);
    lv_obj_set_align(ui_TpsGauge, LV_ALIGN_CENTER);
    
    lv_arc_set_range(ui_TpsGauge, 0, 100);
    lv_arc_set_value(ui_TpsGauge, 68);
    lv_arc_set_bg_angles(ui_TpsGauge, 135, 45);
    lv_obj_set_style_arc_color(ui_TpsGauge, lv_color_hex(COLOR_TPS), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_TpsGauge, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_TpsGauge, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_TpsGauge, lv_color_hex(COLOR_BORDER), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(ui_TpsGauge, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(ui_TpsGauge, LV_OBJ_FLAG_CLICKABLE);

    // TPS Labels
    lv_obj_t *tps_title = lv_label_create(ui_TpsGauge);
    lv_obj_set_align(tps_title, LV_ALIGN_CENTER);
    lv_obj_set_y(tps_title, -40);
    lv_label_set_text(tps_title, "TPS");
    lv_obj_set_style_text_color(tps_title, lv_color_hex(COLOR_TEXT_PRIMARY), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_TpsValueLabel = lv_label_create(ui_TpsGauge);
    lv_obj_set_align(ui_TpsValueLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_TpsValueLabel, "68");
    lv_obj_set_style_text_color(ui_TpsValueLabel, lv_color_hex(COLOR_TPS), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *tps_unit = lv_label_create(ui_TpsGauge);
    lv_obj_set_align(tps_unit, LV_ALIGN_CENTER);
    lv_obj_set_y(tps_unit, 30);
    lv_label_set_text(tps_unit, "%");
    lv_obj_set_style_text_color(tps_unit, lv_color_hex(COLOR_TEXT_SECONDARY), LV_PART_MAIN | LV_STATE_DEFAULT);
}

void ui_create_rpm_gauge(void)
{
    // RPM Gauge - Bottom Left
    ui_RpmGauge = lv_arc_create(ui_MainScreen);
    lv_obj_set_width(ui_RpmGauge, 200);
    lv_obj_set_height(ui_RpmGauge, 200);
    lv_obj_set_x(ui_RpmGauge, -260);
    lv_obj_set_y(ui_RpmGauge, 320);
    lv_obj_set_align(ui_RpmGauge, LV_ALIGN_CENTER);
    
    lv_arc_set_range(ui_RpmGauge, 0, 7000);
    lv_arc_set_value(ui_RpmGauge, 3500);
    lv_arc_set_bg_angles(ui_RpmGauge, 135, 45);
    lv_obj_set_style_arc_color(ui_RpmGauge, lv_color_hex(COLOR_RPM), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_RpmGauge, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_RpmGauge, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_RpmGauge, lv_color_hex(COLOR_BORDER), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(ui_RpmGauge, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(ui_RpmGauge, LV_OBJ_FLAG_CLICKABLE);

    // RPM Labels
    lv_obj_t *rpm_title = lv_label_create(ui_RpmGauge);
    lv_obj_set_align(rpm_title, LV_ALIGN_CENTER);
    lv_obj_set_y(rpm_title, -40);
    lv_label_set_text(rpm_title, "RPM");
    lv_obj_set_style_text_color(rpm_title, lv_color_hex(COLOR_TEXT_PRIMARY), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_RpmValueLabel = lv_label_create(ui_RpmGauge);
    lv_obj_set_align(ui_RpmValueLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_RpmValueLabel, "3500");
    lv_obj_set_style_text_color(ui_RpmValueLabel, lv_color_hex(COLOR_RPM), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *rpm_unit = lv_label_create(ui_RpmGauge);
    lv_obj_set_align(rpm_unit, LV_ALIGN_CENTER);
    lv_obj_set_y(rpm_unit, 30);
    lv_label_set_text(rpm_unit, "RPM");
    lv_obj_set_style_text_color(rpm_unit, lv_color_hex(COLOR_TEXT_SECONDARY), LV_PART_MAIN | LV_STATE_DEFAULT);
}

void ui_create_target_gauge(void)
{
    // Target Boost Gauge - Bottom Center
    ui_TargetGauge = lv_arc_create(ui_MainScreen);
    lv_obj_set_width(ui_TargetGauge, 200);
    lv_obj_set_height(ui_TargetGauge, 200);
    lv_obj_set_x(ui_TargetGauge, 0);
    lv_obj_set_y(ui_TargetGauge, 320);
    lv_obj_set_align(ui_TargetGauge, LV_ALIGN_CENTER);
    
    lv_arc_set_range(ui_TargetGauge, 100, 250);
    lv_arc_set_value(ui_TargetGauge, 180);
    lv_arc_set_bg_angles(ui_TargetGauge, 135, 45);
    lv_obj_set_style_arc_color(ui_TargetGauge, lv_color_hex(COLOR_TARGET), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_TargetGauge, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_TargetGauge, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_TargetGauge, lv_color_hex(COLOR_BORDER), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(ui_TargetGauge, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(ui_TargetGauge, LV_OBJ_FLAG_CLICKABLE);

    // Target Labels
    lv_obj_t *target_title = lv_label_create(ui_TargetGauge);
    lv_obj_set_align(target_title, LV_ALIGN_CENTER);
    lv_obj_set_y(target_title, -40);
    lv_label_set_text(target_title, "Target");
    lv_obj_set_style_text_color(target_title, lv_color_hex(COLOR_TEXT_PRIMARY), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_TargetValueLabel = lv_label_create(ui_TargetGauge);
    lv_obj_set_align(ui_TargetValueLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_TargetValueLabel, "180");
    lv_obj_set_style_text_color(ui_TargetValueLabel, lv_color_hex(COLOR_TARGET), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *target_unit = lv_label_create(ui_TargetGauge);
    lv_obj_set_align(target_unit, LV_ALIGN_CENTER);
    lv_obj_set_y(target_unit, 30);
    lv_label_set_text(target_unit, "kPa");
    lv_obj_set_style_text_color(target_unit, lv_color_hex(COLOR_TEXT_SECONDARY), LV_PART_MAIN | LV_STATE_DEFAULT);
}

void ui_create_tcu_panel(void)
{
    // TCU Status Panel - Bottom Right
    ui_TcuPanel = lv_obj_create(ui_MainScreen);
    lv_obj_set_width(ui_TcuPanel, 200);
    lv_obj_set_height(ui_TcuPanel, 200);
    lv_obj_set_x(ui_TcuPanel, 260);
    lv_obj_set_y(ui_TcuPanel, 320);
    lv_obj_set_align(ui_TcuPanel, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_TcuPanel, lv_color_hex(COLOR_CARD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_TcuPanel, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_TcuPanel, lv_color_hex(COLOR_SUCCESS), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_TcuPanel, 15, LV_PART_MAIN | LV_STATE_DEFAULT);

    // TCU Labels
    lv_obj_t *tcu_title = lv_label_create(ui_TcuPanel);
    lv_obj_set_align(tcu_title, LV_ALIGN_CENTER);
    lv_obj_set_y(tcu_title, -40);
    lv_label_set_text(tcu_title, "TCU Status");
    lv_obj_set_style_text_color(tcu_title, lv_color_hex(COLOR_TEXT_PRIMARY), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_TcuStatusLabel = lv_label_create(ui_TcuPanel);
    lv_obj_set_align(ui_TcuStatusLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_TcuStatusLabel, "Normal");
    lv_obj_set_style_text_color(ui_TcuStatusLabel, lv_color_hex(COLOR_SUCCESS), LV_PART_MAIN | LV_STATE_DEFAULT);

    // TCU Status Indicator
    lv_obj_t *tcu_indicator = lv_obj_create(ui_TcuPanel);
    lv_obj_set_width(tcu_indicator, 20);
    lv_obj_set_height(tcu_indicator, 20);
    lv_obj_set_align(tcu_indicator, LV_ALIGN_CENTER);
    lv_obj_set_y(tcu_indicator, 40);
    lv_obj_set_style_bg_color(tcu_indicator, lv_color_hex(COLOR_SUCCESS), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(tcu_indicator, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tcu_indicator, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

// Update functions for real-time data
void ui_update_map_pressure(uint16_t value)
{
    lv_arc_set_value(ui_MapGauge, value);
    lv_label_set_text_fmt(ui_MapValueLabel, "%d", value);
}

void ui_update_wastegate_position(uint8_t value)
{
    lv_arc_set_value(ui_WastegateGauge, value);
    lv_label_set_text_fmt(ui_WastegateValueLabel, "%d", value);
}

void ui_update_tps_position(uint8_t value)
{
    lv_arc_set_value(ui_TpsGauge, value);
    lv_label_set_text_fmt(ui_TpsValueLabel, "%d", value);
}

void ui_update_engine_rpm(uint16_t value)
{
    lv_arc_set_value(ui_RpmGauge, value);
    lv_label_set_text_fmt(ui_RpmValueLabel, "%d", value);
    
    // Warning colors for high RPM
    if (value > 6500) {
        lv_obj_set_style_arc_color(ui_RpmGauge, lv_color_hex(COLOR_DANGER), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    } else if (value > 6000) {
        lv_obj_set_style_arc_color(ui_RpmGauge, lv_color_hex(COLOR_WARNING), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    } else {
        lv_obj_set_style_arc_color(ui_RpmGauge, lv_color_hex(COLOR_RPM), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    }
}

void ui_update_target_boost(uint16_t value)
{
    lv_arc_set_value(ui_TargetGauge, value);
    lv_label_set_text_fmt(ui_TargetValueLabel, "%d", value);
}

void ui_update_tcu_status(bool protection_active, bool limp_mode)
{
    if (limp_mode) {
        lv_label_set_text(ui_TcuStatusLabel, "Limp Mode");
        lv_obj_set_style_text_color(ui_TcuStatusLabel, lv_color_hex(COLOR_DANGER), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_TcuPanel, lv_color_hex(COLOR_DANGER), LV_PART_MAIN | LV_STATE_DEFAULT);
    } else if (protection_active) {
        lv_label_set_text(ui_TcuStatusLabel, "Protection");
        lv_obj_set_style_text_color(ui_TcuStatusLabel, lv_color_hex(COLOR_WARNING), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_TcuPanel, lv_color_hex(COLOR_WARNING), LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_label_set_text(ui_TcuStatusLabel, "Normal");
        lv_obj_set_style_text_color(ui_TcuStatusLabel, lv_color_hex(COLOR_SUCCESS), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(ui_TcuPanel, lv_color_hex(COLOR_SUCCESS), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

void ui_update_connection_status(bool connected)
{
    if (connected) {
        lv_label_set_text(ui_ConnectionStatus, "Connected");
        lv_obj_set_style_text_color(ui_ConnectionStatus, lv_color_hex(COLOR_SUCCESS), LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_label_set_text(ui_ConnectionStatus, "Disconnected");
        lv_obj_set_style_text_color(ui_ConnectionStatus, lv_color_hex(COLOR_DANGER), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}