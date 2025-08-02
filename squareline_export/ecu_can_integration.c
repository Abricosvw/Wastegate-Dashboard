/**
 * CAN Bus Integration for ECU Dashboard
 * Handles CAN message parsing and data extraction
 */

#include "ecu_can_integration.h"
#include "ecu_data_structures.h"
#include <string.h>

// CAN message IDs
#define CAN_TCU_DATA_ID         0x440
#define CAN_ECU_DATA_ID         0x380
#define CAN_BOOST_CONTROL_ID    0x200

// TCU data structure (from CAN ID 0x440)
typedef struct {
    uint8_t torque_request;      // Byte 3: Torque request (0-255 -> 0-100%)
    uint8_t protection_flags;    // Byte 1: Protection status flags
    uint8_t limp_mode_status;    // Byte 5: Limp mode status (bits 4-7)
    uint8_t gear_position;       // Byte 0: Current gear
    uint16_t input_speed;        // Bytes 6-7: Input shaft speed
} tcu_can_data_t;

// ECU data structure (from CAN ID 0x380)
typedef struct {
    uint16_t engine_rpm;         // Bytes 0-1: Engine RPM
    uint16_t map_pressure;       // Bytes 2-3: MAP pressure (scaled)
    uint8_t tps_position;        // Byte 4: TPS position
    uint8_t coolant_temp;        // Byte 5: Coolant temperature
    uint16_t boost_target;       // Bytes 6-7: Target boost pressure
} ecu_can_data_t;

// Boost control data (from CAN ID 0x200)
typedef struct {
    uint8_t wastegate_position;  // Byte 0: Wastegate position (0-255 -> 0-100%)
    uint8_t boost_control_mode;  // Byte 1: Control mode
    uint16_t measured_boost;     // Bytes 2-3: Measured boost pressure
    uint16_t target_boost;       // Bytes 4-5: Target boost pressure
    uint8_t pid_output;          // Byte 6: PID controller output
    uint8_t safety_flags;        // Byte 7: Safety and protection flags
} boost_control_can_data_t;

// Global variables
static ecu_data_t current_ecu_data = {0};
static uint32_t last_update_time = 0;
static bool data_valid = false;

// Helper function to extract bit range from CAN data
static uint32_t get_bit_range(const uint8_t* data, uint8_t bit_index, uint8_t bit_width)
{
    uint8_t byte_index = bit_index >> 3;
    uint8_t shift = bit_index - (byte_index << 3);
    uint32_t value = data[byte_index];
    
    if (shift + bit_width > 8) {
        value |= (data[byte_index + 1] << 8);
    }
    if (shift + bit_width > 16) {
        value |= (data[byte_index + 2] << 16);
    }
    
    uint32_t mask = (1U << bit_width) - 1;
    return (value >> shift) & mask;
}

// Parse TCU CAN message (ID 0x440)
static void parse_tcu_data(const uint8_t* data, uint8_t length)
{
    if (length < 6) return;
    
    // Extract torque request (Byte 3, scaled 0-255 -> 0-100%)
    uint8_t raw_torque = data[3];
    if (raw_torque == 255) {
        // Invalid data
        current_ecu_data.torque_request = 0;
    } else {
        current_ecu_data.torque_request = (float)raw_torque * 0.39f; // Scale factor from Lua script
    }
    
    // Extract protection status (Byte 1, bit 1)
    uint8_t protection_status = get_bit_range(data, 9, 1);
    current_ecu_data.tcu_protection_active = (protection_status == 1);
    
    // Extract limp mode status (Byte 5, bits 4-7)
    uint8_t limp_mode_status = get_bit_range(data, 40, 4);
    current_ecu_data.tcu_limp_mode = (limp_mode_status != 5 && limp_mode_status != 6 && 
                                      limp_mode_status != 7 && limp_mode_status > 0);
}

// Parse ECU CAN message (ID 0x380)
static void parse_ecu_data(const uint8_t* data, uint8_t length)
{
    if (length < 8) return;
    
    // Extract engine RPM (Bytes 0-1, little endian)
    current_ecu_data.engine_rpm = (float)((data[1] << 8) | data[0]);
    
    // Extract MAP pressure (Bytes 2-3, scaled to kPa)
    uint16_t raw_map = (data[3] << 8) | data[2];
    current_ecu_data.map_pressure = (float)raw_map * 0.1f; // Scale factor for kPa
    
    // Extract TPS position (Byte 4, scaled 0-255 -> 0-100%)
    current_ecu_data.tps_position = (float)data[4] * 0.392f; // Scale factor
}

// Parse boost control CAN message (ID 0x200)
static void parse_boost_control_data(const uint8_t* data, uint8_t length)
{
    if (length < 8) return;
    
    // Extract wastegate position (Byte 0, scaled 0-255 -> 0-100%)
    current_ecu_data.wastegate_position = (float)data[0] * 0.392f;
    
    // Extract target boost (Bytes 4-5, scaled to kPa)
    uint16_t raw_target = (data[5] << 8) | data[4];
    current_ecu_data.target_boost = (float)raw_target * 0.1f;
}

// Main CAN message handler
void can_message_handler(uint32_t can_id, const uint8_t* data, uint8_t length)
{
    switch (can_id) {
        case CAN_TCU_DATA_ID:
            parse_tcu_data(data, length);
            break;
            
        case CAN_ECU_DATA_ID:
            parse_ecu_data(data, length);
            break;
            
        case CAN_BOOST_CONTROL_ID:
            parse_boost_control_data(data, length);
            break;
            
        default:
            // Unknown CAN ID
            return;
    }
    
    // Update timestamp and validity
    last_update_time = lv_tick_get();
    current_ecu_data.timestamp = last_update_time;
    data_valid = true;
}

// Initialize CAN interface
void can_interface_init(void)
{
    // Initialize CAN hardware (implementation depends on MCU)
    // This is a placeholder for actual CAN initialization
    
    // Set up CAN filters for our message IDs
    // can_set_filter(CAN_TCU_DATA_ID, 0x7FF);
    // can_set_filter(CAN_ECU_DATA_ID, 0x7FF);
    // can_set_filter(CAN_BOOST_CONTROL_ID, 0x7FF);
    
    // Enable CAN interrupts
    // can_enable_interrupts();
    
    // Initialize data structure
    memset(&current_ecu_data, 0, sizeof(current_ecu_data));
    current_ecu_data.map_pressure = 100.0f;      // Default atmospheric
    current_ecu_data.wastegate_position = 50.0f;  // Default mid-position
    current_ecu_data.engine_rpm = 800.0f;         // Default idle RPM
    current_ecu_data.target_boost = 120.0f;       // Default target
    
    data_valid = false;
    last_update_time = 0;
}

// Get current ECU data
ecu_data_t* ecu_get_current_data(void)
{
    return &current_ecu_data;
}

// Check if data is valid and recent
bool ecu_data_is_fresh(uint32_t max_age_ms)
{
    if (!data_valid) return false;
    
    uint32_t current_time = lv_tick_get();
    uint32_t age = current_time - last_update_time;
    
    return (age <= max_age_ms);
}

// Simulate CAN data for testing (when no real CAN bus available)
void simulate_can_data(void)
{
    static uint32_t last_sim_time = 0;
    static float sim_phase = 0.0f;
    
    uint32_t current_time = lv_tick_get();
    
    // Update simulation every 50ms (20Hz)
    if (current_time - last_sim_time >= 50) {
        last_sim_time = current_time;
        sim_phase += 0.1f;
        
        // Simulate realistic ECU data
        current_ecu_data.engine_rpm = 1500.0f + sinf(sim_phase * 0.3f) * 500.0f;
        current_ecu_data.map_pressure = 120.0f + sinf(sim_phase * 0.5f) * 30.0f;
        current_ecu_data.tps_position = 20.0f + sinf(sim_phase * 0.8f) * 40.0f;
        current_ecu_data.wastegate_position = 50.0f + sinf(sim_phase * 0.4f) * 20.0f;
        current_ecu_data.target_boost = 140.0f + sinf(sim_phase * 0.2f) * 20.0f;
        current_ecu_data.torque_request = 30.0f + sinf(sim_phase * 0.6f) * 25.0f;
        
        // Simulate protection modes occasionally
        if ((int)(sim_phase * 10) % 100 < 5) {
            current_ecu_data.tcu_protection_active = true;
        } else {
            current_ecu_data.tcu_protection_active = false;
        }
        
        if ((int)(sim_phase * 10) % 200 < 3) {
            current_ecu_data.tcu_limp_mode = true;
        } else {
            current_ecu_data.tcu_limp_mode = false;
        }
        
        current_ecu_data.timestamp = current_time;
        data_valid = true;
        last_update_time = current_time;
    }
}

// CAN transmit function for sending commands back to ECU
void can_send_boost_command(float target_boost, uint8_t control_mode)
{
    uint8_t tx_data[8] = {0};
    
    // Prepare boost control command
    uint16_t scaled_target = (uint16_t)(target_boost * 10.0f);
    tx_data[0] = control_mode;
    tx_data[1] = 0; // Reserved
    tx_data[2] = scaled_target & 0xFF;
    tx_data[3] = (scaled_target >> 8) & 0xFF;
    tx_data[4] = 0; // PID parameters
    tx_data[5] = 0;
    tx_data[6] = 0;
    tx_data[7] = 0;
    
    // Send CAN message (implementation depends on CAN driver)
    // can_transmit(0x201, tx_data, 8);
}

// Error handling for CAN bus errors
void can_error_handler(uint32_t error_code)
{
    // Handle different CAN error types
    switch (error_code) {
        case CAN_ERROR_BUS_OFF:
            // Reset CAN controller
            data_valid = false;
            break;
            
        case CAN_ERROR_PASSIVE:
            // Log warning but continue
            break;
            
        case CAN_ERROR_TIMEOUT:
            // Mark data as stale
            if (lv_tick_get() - last_update_time > 1000) {
                data_valid = false;
            }
            break;
            
        default:
            break;
    }
}

// Periodic maintenance function
void can_interface_task(void)
{
    // Check for data timeout
    if (data_valid && (lv_tick_get() - last_update_time > 500)) {
        data_valid = false;
    }
    
    // In simulation mode, generate test data
    #ifdef SIMULATION_MODE
    simulate_can_data();
    #endif
}