/**
 * Main Integration Example for ECU Dashboard with SquareLine Studio
 * Shows how to integrate all components in a typical embedded application
 */

#include "ui.h"
#include "ecu_can_integration.h"
#include "ecu_data_structures.h"
#include "lvgl.h"

// Application configuration
#define UPDATE_PERIOD_MS        50      // 20Hz update rate
#define DATA_TIMEOUT_MS         500     // Data considered stale after 500ms
#define DISPLAY_WIDTH          800      // Adjust for your display
#define DISPLAY_HEIGHT         480      // Adjust for your display

// Global variables
static lv_timer_t *main_update_timer;
static system_settings_t system_settings;
static bool system_initialized = false;

// Forward declarations
static void main_update_task(lv_timer_t *timer);
static void initialize_system_settings(void);
static void handle_system_alerts(void);
static void update_ui_with_ecu_data(void);

/**
 * Main application initialization
 * Call this after LVGL and display driver initialization
 */
void ecu_dashboard_init(void)
{
    // Initialize system settings
    initialize_system_settings();
    
    // Initialize CAN interface
    can_interface_init();
    
    // Initialize UI screens and events
    ui_init();
    ui_events_init();
    
    // Create main update timer
    main_update_timer = lv_timer_create(main_update_task, UPDATE_PERIOD_MS, NULL);
    
    // Set initial display settings
    display_settings_t default_settings = {
        .gauge_size = 1,            // Medium
        .columns = 3,               // 3 columns
        .show_titles = true,
        .show_values = true,
        .show_targets = true,
        .compact_mode = false,
        .gauge_style = 0,           // Circular
        .visible_gauges = 0x3F      // All gauges visible
    };
    ui_set_display_settings(&default_settings);
    
    system_initialized = true;
}

/**
 * Main application task - called every UPDATE_PERIOD_MS
 */
static void main_update_task(lv_timer_t *timer)
{
    (void)timer;
    
    if (!system_initialized) return;
    
    // Run CAN interface maintenance
    can_interface_task();
    
    // Update UI with latest ECU data
    update_ui_with_ecu_data();
    
    // Handle system alerts and warnings
    handle_system_alerts();
}

/**
 * Update UI with current ECU data
 */
static void update_ui_with_ecu_data(void)
{
    ecu_data_t *ecu_data = ecu_get_current_data();
    
    if (ecu_data_is_fresh(DATA_TIMEOUT_MS)) {
        // Data is fresh - update UI
        ui_set_ecu_data(ecu_data);
        ui_set_connection_status(true, "Connected");
    } else {
        // Data is stale - show disconnected
        ui_set_connection_status(false, "No Data");
    }
}

/**
 * Handle system alerts based on ECU data
 */
static void handle_system_alerts(void)
{
    ecu_data_t *ecu_data = ecu_get_current_data();
    
    if (!ecu_data_is_fresh(DATA_TIMEOUT_MS)) {
        return; // No valid data
    }
    
    // Check for over-boost condition
    if (ecu_data->map_pressure > system_settings.max_boost_limit) {
        // Trigger over-boost alert
        #ifdef AUDIO_ALERTS_ENABLED
        if (system_settings.audio_alerts_enabled) {
            // play_alert_sound(ALERT_OVERBOOST);
        }
        #endif
        
        // Flash warning on display
        // set_warning_led(true);
    }
    
    // Check for over-rev condition
    if (ecu_data->engine_rpm > system_settings.max_rpm_limit) {
        // Trigger over-rev alert
        #ifdef AUDIO_ALERTS_ENABLED
        if (system_settings.audio_alerts_enabled) {
            // play_alert_sound(ALERT_OVERREV);
        }
        #endif
    }
    
    // Check for TCU protection/limp mode
    if (ecu_data->tcu_limp_mode) {
        // Visual indication already handled by UI
        // Additional actions can be added here
    }
}

/**
 * Initialize default system settings
 */
static void initialize_system_settings(void)
{
    system_settings.max_boost_limit = 250.0f;      // kPa
    system_settings.max_rpm_limit = 7000.0f;       // RPM
    system_settings.audio_alerts_enabled = true;
    system_settings.ecu_address = "CAN Bus";
    system_settings.update_rate = 20;              // Hz
}

/**
 * Settings update callback from UI
 */
void on_settings_changed(const system_settings_t *new_settings)
{
    if (new_settings) {
        system_settings = *new_settings;
        
        // Save to non-volatile memory if available
        // save_settings_to_flash(&system_settings);
        
        // Apply settings that affect operation
        if (main_update_timer) {
            lv_timer_set_period(main_update_timer, 1000 / system_settings.update_rate);
        }
    }
}

/**
 * Display settings update callback from UI
 */
void on_display_settings_changed(const display_settings_t *new_settings)
{
    ui_set_display_settings(new_settings);
    
    // Save to non-volatile memory if available
    // save_display_settings_to_flash(new_settings);
}

/**
 * Manual boost control command
 */
void send_boost_control_command(float target_boost)
{
    // Clamp to safe limits
    if (target_boost > system_settings.max_boost_limit) {
        target_boost = system_settings.max_boost_limit;
    }
    
    if (target_boost < 100.0f) {
        target_boost = 100.0f; // Minimum atmospheric pressure
    }
    
    // Send CAN command
    can_send_boost_command(target_boost, BOOST_MODE_MANUAL);
}

/**
 * Emergency stop function
 */
void emergency_stop(void)
{
    // Send safe boost command
    can_send_boost_command(100.0f, BOOST_MODE_SAFETY);
    
    // Update UI to show emergency state
    ui_set_connection_status(false, "EMERGENCY");
    
    // Disable normal operation
    if (main_update_timer) {
        lv_timer_pause(main_update_timer);
    }
}

/**
 * Resume normal operation after emergency
 */
void resume_operation(void)
{
    // Re-enable normal operation
    if (main_update_timer) {
        lv_timer_resume(main_update_timer);
    }
    
    // Send automatic mode command
    can_send_boost_command(0.0f, BOOST_MODE_AUTOMATIC);
}

/**
 * Button event handlers for physical buttons (if available)
 */
void button_settings_pressed(void)
{
    // Switch to settings screen
    lv_scr_load_anim(ui_SettingsScreen, LV_SCR_LOAD_ANIM_SLIDE_LEFT, 300, 0, false);
}

void button_back_pressed(void)
{
    // Return to main screen
    lv_scr_load_anim(ui_MainScreen, LV_SCR_LOAD_ANIM_SLIDE_RIGHT, 300, 0, false);
}

void button_emergency_pressed(void)
{
    static bool emergency_active = false;
    
    if (!emergency_active) {
        emergency_stop();
        emergency_active = true;
    } else {
        resume_operation();
        emergency_active = false;
    }
}

/**
 * Data logging function (optional)
 */
void log_ecu_data(void)
{
    ecu_data_t *ecu_data = ecu_get_current_data();
    
    if (ecu_data_is_fresh(DATA_TIMEOUT_MS)) {
        // Log to SD card, flash memory, or send via communication interface
        // This could be CSV format, binary format, or custom protocol
        
        // Example: Write to SD card in CSV format
        /*
        char log_entry[256];
        snprintf(log_entry, sizeof(log_entry),
                "%lu,%.1f,%.1f,%.1f,%.0f,%.1f,%d,%d,%.1f\n",
                ecu_data->timestamp,
                ecu_data->map_pressure,
                ecu_data->wastegate_position,
                ecu_data->tps_position,
                ecu_data->engine_rpm,
                ecu_data->target_boost,
                ecu_data->tcu_protection_active ? 1 : 0,
                ecu_data->tcu_limp_mode ? 1 : 0,
                ecu_data->torque_request);
        
        sd_card_write(log_entry, strlen(log_entry));
        */
    }
}

/**
 * Application cleanup
 */
void ecu_dashboard_cleanup(void)
{
    if (main_update_timer) {
        lv_timer_del(main_update_timer);
        main_update_timer = NULL;
    }
    
    ui_events_cleanup();
    system_initialized = false;
}

/**
 * Power management callback
 */
void on_power_mode_change(uint8_t power_mode)
{
    switch (power_mode) {
        case 0: // Normal operation
            if (main_update_timer) {
                lv_timer_resume(main_update_timer);
            }
            break;
            
        case 1: // Low power mode
            if (main_update_timer) {
                lv_timer_set_period(main_update_timer, 200); // Reduce to 5Hz
            }
            break;
            
        case 2: // Sleep mode
            if (main_update_timer) {
                lv_timer_pause(main_update_timer);
            }
            // Turn off display backlight
            break;
    }
}

/**
 * Error recovery function
 */
void handle_system_error(uint32_t error_code)
{
    switch (error_code) {
        case CAN_ERROR_BUS_OFF:
            // Attempt to restart CAN interface
            can_interface_init();
            break;
            
        case CAN_ERROR_TIMEOUT:
            // Data timeout already handled in main task
            break;
            
        default:
            // Log error and continue
            break;
    }
}