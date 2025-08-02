#ifndef ECU_DATA_STRUCTURES_H
#define ECU_DATA_STRUCTURES_H

#include <stdint.h>
#include <stdbool.h>

// ECU Data Structure for real-time dashboard
typedef struct {
    float map_pressure;          // MAP sensor pressure in kPa (100-250)
    float wastegate_position;    // Wastegate position in % (0-100)
    float tps_position;          // Throttle position in % (0-100)
    float engine_rpm;            // Engine RPM (0-7000)
    float target_boost;          // Target boost pressure in kPa (100-250)
    bool tcu_protection_active;  // TCU protection status
    bool tcu_limp_mode;          // TCU limp mode status
    float torque_request;        // Torque request in % (0-100)
    uint32_t timestamp;          // Timestamp in milliseconds
} ecu_data_t;

// Gauge configuration structure
typedef struct {
    const char* title;           // Gauge title
    const char* subtitle;        // Gauge subtitle
    const char* unit;            // Unit of measurement
    float min_value;             // Minimum value
    float max_value;             // Maximum value
    float warning_threshold;     // Warning threshold
    float danger_threshold;      // Danger threshold
    uint32_t color;              // Primary color (RGB565 or ARGB8888)
    uint32_t warning_color;      // Warning color
    uint32_t danger_color;       // Danger color
} gauge_config_t;

// Display settings structure
typedef struct {
    uint8_t gauge_size;          // 0=small, 1=medium, 2=large, 3=xlarge
    uint8_t columns;             // Number of columns (1-6)
    bool show_titles;            // Show gauge titles
    bool show_values;            // Show gauge values
    bool show_targets;           // Show target indicators
    bool compact_mode;           // Compact display mode
    uint8_t gauge_style;         // 0=circular, 1=arc, 2=linear
    uint8_t visible_gauges;      // Bitmask for visible gauges
} display_settings_t;

// Connection status structure
typedef struct {
    bool connected;              // Connection status
    const char* message;         // Status message
    uint32_t last_update;        // Last update timestamp
    uint16_t data_rate;          // Data rate in Hz
} connection_status_t;

// System settings structure
typedef struct {
    float max_boost_limit;       // Maximum boost limit in kPa
    float max_rpm_limit;         // Maximum RPM limit
    bool audio_alerts_enabled;   // Audio alerts enable/disable
    const char* ecu_address;     // ECU IP address
    uint8_t update_rate;         // Update rate in Hz
} system_settings_t;

// Data stream entry structure
typedef struct {
    uint32_t timestamp;          // Entry timestamp
    const char* message;         // Log message
    uint8_t type;                // 0=info, 1=warning, 2=success, 3=error
} data_stream_entry_t;

// Gauge visibility bitmask definitions
#define GAUGE_MAP_VISIBLE       (1 << 0)
#define GAUGE_WASTEGATE_VISIBLE (1 << 1)
#define GAUGE_TPS_VISIBLE       (1 << 2)
#define GAUGE_RPM_VISIBLE       (1 << 3)
#define GAUGE_TARGET_VISIBLE    (1 << 4)
#define GAUGE_TCU_VISIBLE       (1 << 5)

// Color definitions (RGB565 format for embedded displays)
#define COLOR_BACKGROUND        0x0000  // Black
#define COLOR_CARD              0x18C3  // Dark gray
#define COLOR_TEXT_PRIMARY      0xFFFF  // White
#define COLOR_TEXT_SECONDARY    0xA514  // Light gray
#define COLOR_ACCENT            0x007F  // Blue
#define COLOR_SUCCESS           0x07E0  // Green
#define COLOR_WARNING           0xFD20  // Orange
#define COLOR_DANGER            0xF800  // Red
#define COLOR_YELLOW            0xFFE0  // Yellow

// Function prototypes for data handling
void ecu_data_init(void);
void ecu_data_update(const ecu_data_t* new_data);
ecu_data_t* ecu_data_get_current(void);
bool ecu_data_is_valid(void);
uint32_t ecu_data_get_age_ms(void);

// Function prototypes for gauge management
void gauge_init(void);
void gauge_set_config(uint8_t gauge_id, const gauge_config_t* config);
void gauge_update_value(uint8_t gauge_id, float value);
void gauge_set_target(uint8_t gauge_id, float target);

// Function prototypes for display settings
void display_settings_init(void);
void display_settings_set(const display_settings_t* settings);
display_settings_t* display_settings_get(void);
void display_settings_save_to_flash(void);
void display_settings_load_from_flash(void);

// Function prototypes for connection management
void connection_init(void);
void connection_set_status(bool connected, const char* message);
connection_status_t* connection_get_status(void);
void connection_update_data_rate(uint16_t rate);

// Function prototypes for data logging
void data_stream_init(void);
void data_stream_add_entry(const char* message, uint8_t type);
data_stream_entry_t* data_stream_get_entries(uint16_t* count);
void data_stream_clear(void);

#endif // ECU_DATA_STRUCTURES_H