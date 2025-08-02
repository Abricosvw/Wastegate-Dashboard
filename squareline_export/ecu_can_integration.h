/**
 * CAN Bus Integration Header for ECU Dashboard
 * Handles CAN message parsing and data extraction
 */

#ifndef ECU_CAN_INTEGRATION_H
#define ECU_CAN_INTEGRATION_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "lvgl.h"
#include "ecu_data_structures.h"

#ifdef __cplusplus
extern "C" {
#endif

// CAN error codes
#define CAN_ERROR_NONE          0x00
#define CAN_ERROR_BUS_OFF       0x01
#define CAN_ERROR_PASSIVE       0x02
#define CAN_ERROR_TIMEOUT       0x03
#define CAN_ERROR_OVERRUN       0x04

// CAN message IDs
#define CAN_TCU_DATA_ID         0x440
#define CAN_ECU_DATA_ID         0x380
#define CAN_BOOST_CONTROL_ID    0x200
#define CAN_BOOST_COMMAND_ID    0x201

// Boost control modes
#define BOOST_MODE_MANUAL       0x00
#define BOOST_MODE_AUTOMATIC    0x01
#define BOOST_MODE_SAFETY       0x02
#define BOOST_MODE_CALIBRATION  0x03

// Function prototypes

/**
 * Initialize CAN interface and configure filters
 */
void can_interface_init(void);

/**
 * Main CAN message handler - called by CAN interrupt
 * @param can_id CAN message ID
 * @param data Pointer to CAN data bytes
 * @param length Number of data bytes (0-8)
 */
void can_message_handler(uint32_t can_id, const uint8_t* data, uint8_t length);

/**
 * Get pointer to current ECU data structure
 * @return Pointer to current ECU data
 */
ecu_data_t* ecu_get_current_data(void);

/**
 * Check if ECU data is fresh and valid
 * @param max_age_ms Maximum acceptable age in milliseconds
 * @return true if data is valid and within age limit
 */
bool ecu_data_is_fresh(uint32_t max_age_ms);

/**
 * Send boost control command to ECU
 * @param target_boost Target boost pressure in kPa
 * @param control_mode Control mode (manual/auto/safety)
 */
void can_send_boost_command(float target_boost, uint8_t control_mode);

/**
 * Handle CAN bus errors
 * @param error_code Error code from CAN controller
 */
void can_error_handler(uint32_t error_code);

/**
 * Periodic maintenance task for CAN interface
 * Should be called regularly (e.g., every 10ms)
 */
void can_interface_task(void);

/**
 * Simulate CAN data for testing without real CAN bus
 * Only used in simulation mode
 */
void simulate_can_data(void);

// Inline helper functions

/**
 * Convert raw torque value to percentage
 * @param raw_value Raw value from CAN (0-255)
 * @return Torque percentage (0.0-100.0)
 */
static inline float convert_torque_to_percent(uint8_t raw_value)
{
    return (raw_value == 255) ? 0.0f : (float)raw_value * 0.39f;
}

/**
 * Convert raw pressure value to kPa
 * @param raw_value Raw 16-bit value from CAN
 * @return Pressure in kPa
 */
static inline float convert_pressure_to_kpa(uint16_t raw_value)
{
    return (float)raw_value * 0.1f;
}

/**
 * Convert raw position value to percentage
 * @param raw_value Raw value from CAN (0-255)
 * @return Position percentage (0.0-100.0)
 */
static inline float convert_position_to_percent(uint8_t raw_value)
{
    return (float)raw_value * 0.392f;
}

/**
 * Check if TCU is in protection mode
 * @param protection_flags Flags from CAN message
 * @return true if protection is active
 */
static inline bool is_tcu_protection_active(uint8_t protection_flags)
{
    return (protection_flags & 0x02) != 0; // Bit 1
}

/**
 * Check if TCU is in limp mode
 * @param limp_status Limp mode status from CAN
 * @return true if in limp mode
 */
static inline bool is_tcu_limp_mode(uint8_t limp_status)
{
    return (limp_status != 5 && limp_status != 6 && limp_status != 7 && limp_status > 0);
}

#ifdef __cplusplus
}
#endif

#endif // ECU_CAN_INTEGRATION_H