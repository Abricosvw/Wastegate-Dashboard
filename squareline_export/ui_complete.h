/**
 * Complete UI header for SquareLine Studio
 * ECU Dashboard with 6 automotive gauges
 */

#ifndef UI_COMPLETE_H
#define UI_COMPLETE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

// Screen objects
extern lv_obj_t *ui_MainScreen;
extern lv_obj_t *ui_HeaderPanel;
extern lv_obj_t *ui_TitleLabel;
extern lv_obj_t *ui_ConnectionStatus;
extern lv_obj_t *ui_GaugeGrid;

// Gauge objects
extern lv_obj_t *ui_MapGauge;
extern lv_obj_t *ui_WastegateGauge;
extern lv_obj_t *ui_TpsGauge;
extern lv_obj_t *ui_RpmGauge;
extern lv_obj_t *ui_TargetGauge;
extern lv_obj_t *ui_TcuPanel;

// Value labels
extern lv_obj_t *ui_MapValueLabel;
extern lv_obj_t *ui_WastegateValueLabel;
extern lv_obj_t *ui_TpsValueLabel;
extern lv_obj_t *ui_RpmValueLabel;
extern lv_obj_t *ui_TargetValueLabel;
extern lv_obj_t *ui_TcuStatusLabel;

// Color definitions for SquareLine Studio
#define COLOR_BACKGROUND    0x0A0A0A  // Very dark background
#define COLOR_CARD          0x1A1A1A  // Dark card background
#define COLOR_TEXT_PRIMARY  0xFFFFFF  // White text
#define COLOR_TEXT_SECONDARY 0x888888 // Gray text
#define COLOR_BORDER        0x333333  // Dark border

// ECU parameter colors (automotive theme)
#define COLOR_MAP           0x00D4FF  // Blue - MAP Pressure
#define COLOR_WASTEGATE     0x00FF88  // Green - Wastegate Position
#define COLOR_TPS           0xFFD700  // Yellow - TPS Position
#define COLOR_RPM           0xFF6B35  // Orange - Engine RPM
#define COLOR_TARGET        0xFFD700  // Yellow - Target Boost
#define COLOR_SUCCESS       0x00FF88  // Green - Normal status
#define COLOR_WARNING       0xFF6B35  // Orange - Warning
#define COLOR_DANGER        0xFF3366  // Red - Danger/Error

// Gauge configuration
#define GAUGE_SIZE_MEDIUM   200       // Standard gauge size
#define GAUGE_ARC_WIDTH     8         // Arc thickness
#define GAUGE_START_ANGLE   135       // Start angle (5 o'clock)
#define GAUGE_END_ANGLE     45        // End angle (1 o'clock)

// ECU parameter ranges
#define MAP_PRESSURE_MIN    100       // kPa
#define MAP_PRESSURE_MAX    250       // kPa
#define WASTEGATE_MIN       0         // %
#define WASTEGATE_MAX       100       // %
#define TPS_MIN             0         // %
#define TPS_MAX             100       // %
#define ENGINE_RPM_MIN      0         // RPM
#define ENGINE_RPM_MAX      7000      // RPM
#define ENGINE_RPM_WARNING  6000      // RPM warning threshold
#define ENGINE_RPM_DANGER   6500      // RPM danger threshold
#define TARGET_BOOST_MIN    100       // kPa
#define TARGET_BOOST_MAX    250       // kPa

// Grid positioning (for 800x480 screen)
#define GAUGE_GRID_COLS     3
#define GAUGE_GRID_ROWS     2
#define GAUGE_X_OFFSET      260       // Distance from center
#define GAUGE_Y_TOP         90        // Top row Y position
#define GAUGE_Y_BOTTOM      320       // Bottom row Y position

// Main initialization and screen functions
void ui_init(void);
void ui_MainScreen_screen_init(void);

// Gauge creation functions
void ui_create_map_gauge(void);
void ui_create_wastegate_gauge(void);
void ui_create_tps_gauge(void);
void ui_create_rpm_gauge(void);
void ui_create_target_gauge(void);
void ui_create_tcu_panel(void);

// Real-time update functions
void ui_update_map_pressure(uint16_t value);
void ui_update_wastegate_position(uint8_t value);
void ui_update_tps_position(uint8_t value);
void ui_update_engine_rpm(uint16_t value);
void ui_update_target_boost(uint16_t value);
void ui_update_tcu_status(bool protection_active, bool limp_mode);
void ui_update_connection_status(bool connected);

// Event handlers (to be implemented)
void ui_event_connect_clicked(lv_event_t *e);
void ui_event_disconnect_clicked(lv_event_t *e);
void ui_event_settings_clicked(lv_event_t *e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // UI_COMPLETE_H