/**
 * SquareLine Studio UI Screens for ECU Dashboard
 * Generated from web application components
 */

#include "ui.h"
#include "ecu_data_structures.h"

// Main dashboard screen
lv_obj_t *ui_MainScreen;
lv_obj_t *ui_HeaderPanel;
lv_obj_t *ui_TitleLabel;
lv_obj_t *ui_ConnectionStatus;
lv_obj_t *ui_GaugeGrid;
lv_obj_t *ui_StatusBanner;
lv_obj_t *ui_ControlPanel;

// Gauge objects
lv_obj_t *ui_MapPressureGauge;
lv_obj_t *ui_WastegateGauge;
lv_obj_t *ui_TpsGauge;
lv_obj_t *ui_RpmGauge;
lv_obj_t *ui_TargetBoostGauge;
lv_obj_t *ui_TcuStatusPanel;

// Settings screen
lv_obj_t *ui_SettingsScreen;
lv_obj_t *ui_SettingsPanel;
lv_obj_t *ui_SizeSlider;
lv_obj_t *ui_ColumnsSlider;
lv_obj_t *ui_StyleDropdown;

// Data display labels
lv_obj_t *ui_MapValueLabel;
lv_obj_t *ui_WastegateValueLabel;
lv_obj_t *ui_TpsValueLabel;
lv_obj_t *ui_RpmValueLabel;
lv_obj_t *ui_TargetValueLabel;

// Create main dashboard screen
void ui_MainScreen_screen_init(void)
{
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
    lv_obj_set_width(ui_TitleLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_TitleLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_TitleLabel, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_TitleLabel, "Turbo Control Dashboard");
    lv_obj_set_style_text_color(ui_TitleLabel, lv_color_hex(COLOR_TEXT_PRIMARY), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_TitleLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Connection status
    ui_ConnectionStatus = lv_label_create(ui_HeaderPanel);
    lv_obj_set_width(ui_ConnectionStatus, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_ConnectionStatus, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_ConnectionStatus, LV_ALIGN_RIGHT_MID);
    lv_label_set_text(ui_ConnectionStatus, "Disconnected");
    lv_obj_set_style_text_color(ui_ConnectionStatus, lv_color_hex(COLOR_DANGER), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Status banner
    ui_StatusBanner = lv_obj_create(ui_MainScreen);
    lv_obj_set_width(ui_StatusBanner, lv_pct(95));
    lv_obj_set_height(ui_StatusBanner, 50);
    lv_obj_set_x(ui_StatusBanner, 0);
    lv_obj_set_y(ui_StatusBanner, 70);
    lv_obj_set_align(ui_StatusBanner, LV_ALIGN_TOP_MID);
    lv_obj_set_style_bg_color(ui_StatusBanner, lv_color_hex(COLOR_CARD), LV_PART_MAIN | LV_STATE_DEFAULT);

    // Gauge grid container
    ui_GaugeGrid = lv_obj_create(ui_MainScreen);
    lv_obj_set_width(ui_GaugeGrid, lv_pct(95));
    lv_obj_set_height(ui_GaugeGrid, 360);
    lv_obj_set_x(ui_GaugeGrid, 0);
    lv_obj_set_y(ui_GaugeGrid, 130);
    lv_obj_set_align(ui_GaugeGrid, LV_ALIGN_TOP_MID);
    lv_obj_set_flex_flow(ui_GaugeGrid, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_GaugeGrid, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_GaugeGrid, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_opa(ui_GaugeGrid, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_GaugeGrid, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_GaugeGrid, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Create individual gauges
    ui_create_map_pressure_gauge();
    ui_create_wastegate_gauge();
    ui_create_tps_gauge();
    ui_create_rpm_gauge();
    ui_create_target_boost_gauge();
    ui_create_tcu_status_panel();

    // Control panel
    ui_ControlPanel = lv_obj_create(ui_MainScreen);
    lv_obj_set_width(ui_ControlPanel, lv_pct(95));
    lv_obj_set_height(ui_ControlPanel, 80);
    lv_obj_set_align(ui_ControlPanel, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_bg_color(ui_ControlPanel, lv_color_hex(COLOR_CARD), LV_PART_MAIN | LV_STATE_DEFAULT);
}

// Create MAP pressure gauge
void ui_create_map_pressure_gauge(void)
{
    ui_MapPressureGauge = lv_arc_create(ui_GaugeGrid);
    lv_obj_set_width(ui_MapPressureGauge, 180);
    lv_obj_set_height(ui_MapPressureGauge, 180);
    lv_arc_set_range(ui_MapPressureGauge, 100, 250);
    lv_arc_set_value(ui_MapPressureGauge, 100);
    lv_arc_set_bg_angles(ui_MapPressureGauge, 135, 45);
    lv_obj_set_style_arc_color(ui_MapPressureGauge, lv_color_hex(COLOR_ACCENT), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_MapPressureGauge, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_MapPressureGauge, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Value label
    ui_MapValueLabel = lv_label_create(ui_MapPressureGauge);
    lv_obj_set_width(ui_MapValueLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_MapValueLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_MapValueLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_MapValueLabel, "100.0");
    lv_obj_set_style_text_color(ui_MapValueLabel, lv_color_hex(COLOR_ACCENT), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_MapValueLabel, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
}

// Create wastegate position gauge
void ui_create_wastegate_gauge(void)
{
    ui_WastegateGauge = lv_arc_create(ui_GaugeGrid);
    lv_obj_set_width(ui_WastegateGauge, 180);
    lv_obj_set_height(ui_WastegateGauge, 180);
    lv_arc_set_range(ui_WastegateGauge, 0, 100);
    lv_arc_set_value(ui_WastegateGauge, 50);
    lv_arc_set_bg_angles(ui_WastegateGauge, 135, 45);
    lv_obj_set_style_arc_color(ui_WastegateGauge, lv_color_hex(COLOR_SUCCESS), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_WastegateGauge, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_WastegateGauge, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Value label
    ui_WastegateValueLabel = lv_label_create(ui_WastegateGauge);
    lv_obj_set_width(ui_WastegateValueLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_WastegateValueLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_WastegateValueLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WastegateValueLabel, "50.0");
    lv_obj_set_style_text_color(ui_WastegateValueLabel, lv_color_hex(COLOR_SUCCESS), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_WastegateValueLabel, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
}

// Create TPS gauge
void ui_create_tps_gauge(void)
{
    ui_TpsGauge = lv_arc_create(ui_GaugeGrid);
    lv_obj_set_width(ui_TpsGauge, 180);
    lv_obj_set_height(ui_TpsGauge, 180);
    lv_arc_set_range(ui_TpsGauge, 0, 100);
    lv_arc_set_value(ui_TpsGauge, 0);
    lv_arc_set_bg_angles(ui_TpsGauge, 135, 45);
    lv_obj_set_style_arc_color(ui_TpsGauge, lv_color_hex(COLOR_YELLOW), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_TpsGauge, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_TpsGauge, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Value label
    ui_TpsValueLabel = lv_label_create(ui_TpsGauge);
    lv_obj_set_width(ui_TpsValueLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_TpsValueLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_TpsValueLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_TpsValueLabel, "0.0");
    lv_obj_set_style_text_color(ui_TpsValueLabel, lv_color_hex(COLOR_YELLOW), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_TpsValueLabel, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
}

// Create RPM gauge
void ui_create_rpm_gauge(void)
{
    ui_RpmGauge = lv_arc_create(ui_GaugeGrid);
    lv_obj_set_width(ui_RpmGauge, 180);
    lv_obj_set_height(ui_RpmGauge, 180);
    lv_arc_set_range(ui_RpmGauge, 0, 7000);
    lv_arc_set_value(ui_RpmGauge, 800);
    lv_arc_set_bg_angles(ui_RpmGauge, 135, 45);
    lv_obj_set_style_arc_color(ui_RpmGauge, lv_color_hex(COLOR_WARNING), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_RpmGauge, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_RpmGauge, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Value label
    ui_RpmValueLabel = lv_label_create(ui_RpmGauge);
    lv_obj_set_width(ui_RpmValueLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_RpmValueLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_RpmValueLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_RpmValueLabel, "800");
    lv_obj_set_style_text_color(ui_RpmValueLabel, lv_color_hex(COLOR_WARNING), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_RpmValueLabel, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
}

// Create target boost gauge
void ui_create_target_boost_gauge(void)
{
    ui_TargetBoostGauge = lv_arc_create(ui_GaugeGrid);
    lv_obj_set_width(ui_TargetBoostGauge, 180);
    lv_obj_set_height(ui_TargetBoostGauge, 180);
    lv_arc_set_range(ui_TargetBoostGauge, 100, 250);
    lv_arc_set_value(ui_TargetBoostGauge, 120);
    lv_arc_set_bg_angles(ui_TargetBoostGauge, 135, 45);
    lv_obj_set_style_arc_color(ui_TargetBoostGauge, lv_color_hex(COLOR_YELLOW), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_TargetBoostGauge, 8, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_TargetBoostGauge, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

    // Value label
    ui_TargetValueLabel = lv_label_create(ui_TargetBoostGauge);
    lv_obj_set_width(ui_TargetValueLabel, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_TargetValueLabel, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_TargetValueLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_TargetValueLabel, "120.0");
    lv_obj_set_style_text_color(ui_TargetValueLabel, lv_color_hex(COLOR_YELLOW), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_TargetValueLabel, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);
}

// Create TCU status panel
void ui_create_tcu_status_panel(void)
{
    ui_TcuStatusPanel = lv_obj_create(ui_GaugeGrid);
    lv_obj_set_width(ui_TcuStatusPanel, 180);
    lv_obj_set_height(ui_TcuStatusPanel, 180);
    lv_obj_set_style_bg_color(ui_TcuStatusPanel, lv_color_hex(COLOR_CARD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_TcuStatusPanel, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_TcuStatusPanel, lv_color_hex(COLOR_SUCCESS), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_TcuStatusPanel, 90, LV_PART_MAIN | LV_STATE_DEFAULT);
}

// Settings screen initialization
void ui_SettingsScreen_screen_init(void)
{
    ui_SettingsScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_SettingsScreen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_SettingsScreen, lv_color_hex(COLOR_BACKGROUND), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_SettingsPanel = lv_obj_create(ui_SettingsScreen);
    lv_obj_set_width(ui_SettingsPanel, lv_pct(90));
    lv_obj_set_height(ui_SettingsPanel, lv_pct(80));
    lv_obj_set_align(ui_SettingsPanel, LV_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_SettingsPanel, lv_color_hex(COLOR_CARD), LV_PART_MAIN | LV_STATE_DEFAULT);
}

// Initialize all screens
void ui_init(void)
{
    ui_MainScreen_screen_init();
    ui_SettingsScreen_screen_init();
    lv_disp_load_scr(ui_MainScreen);
}