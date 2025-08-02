/*
 * ECU Dashboard for ESP32 with TFT Display and CAN Bus
 * Automotive gauge dashboard with real-time ECU data
 * 
 * Hardware Requirements:
 * - ESP32-S3 or ESP32 DevKit
 * - ILI9341 TFT Display (320x240 or 800x480)
 * - TJA1051 CAN Transceiver 1Mbit/s High Speed
 * 
 * Pin Configuration:
 * TFT Display (SPI):
 * - VCC -> 3.3V
 * - GND -> GND
 * - CS  -> GPIO 15
 * - RST -> GPIO 4
 * - DC  -> GPIO 2
 * - MOSI-> GPIO 23
 * - SCK -> GPIO 18
 * - BLK -> GPIO 32 (Backlight)
 * 
 * TJA1051 CAN Transceiver:
 * - VCC -> 5V (or 3.3V depending on module)
 * - GND -> GND
 * - CTX -> GPIO 21 (CAN TX)
 * - CRX -> GPIO 22 (CAN RX)
 * - CANH -> CAN High Bus
 * - CANL -> CAN Low Bus
 * - S (Silent/Standby) -> GND (Normal mode)
 */

#include "lvgl.h"
#include "ui.h"
#include <TFT_eSPI.h>
#include <SPI.h>
#include <driver/twai.h>  // ESP32 built-in CAN (TWAI) driver
#include <WiFi.h>

// Hardware Configuration
#define TFT_WIDTH  800
#define TFT_HEIGHT 480
#define TFT_BL_PIN 32

// CAN Bus Configuration using ESP32 TWAI (CAN)
#define CAN_TX_PIN   21  // GPIO21 -> TJA1051 CTX
#define CAN_RX_PIN   22  // GPIO22 -> TJA1051 CRX
#define CAN_SPEED    TWAI_TIMING_CONFIG_500KBPS()  // 500 кбит/с
#define CAN_SPEED_1M TWAI_TIMING_CONFIG_1MBPS()    // 1 Мбит/с (если нужно)

// CAN Message IDs
#define TCU_CAN_ID     0x440
#define ECU_CAN_ID     0x380  
#define BOOST_CAN_ID   0x200

// Display and LVGL
TFT_eSPI tft = TFT_eSPI();

// ECU Data Structure
struct ECUData {
  uint16_t mapPressure;      // kPa (100-250)
  uint8_t  wastegatePos;     // % (0-100)
  uint8_t  tpsPosition;      // % (0-100)
  uint16_t engineRpm;        // RPM (0-7000)
  uint16_t targetBoost;      // kPa (100-250)
  bool     tcuProtection;    // TCU protection active
  bool     tcuLimpMode;      // TCU limp mode
  uint8_t  torqueRequest;    // % (0-100)
};

ECUData ecuData = {150, 45, 68, 3500, 180, false, false, 75};

// Timing
unsigned long lastCanUpdate = 0;
unsigned long lastDisplayUpdate = 0;
const unsigned long CAN_UPDATE_INTERVAL = 50;    // 20Hz
const unsigned long DISPLAY_UPDATE_INTERVAL = 50; // 20Hz

// WiFi Credentials (optional for logging)
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

void setup() {
  Serial.begin(115200);
  Serial.println("ECU Dashboard Starting...");
  
  // Initialize display
  initDisplay();
  
  // Initialize LVGL
  initLVGL();
  
  // Initialize CAN Bus
  initCAN();
  
  // Initialize WiFi (optional)
  // initWiFi();
  
  // Create UI
  ui_init();
  
  // Initial data update
  updateDisplayValues();
  
  Serial.println("ECU Dashboard Ready!");
}

void loop() {
  unsigned long currentTime = millis();
  
  // Read CAN messages
  if (currentTime - lastCanUpdate >= CAN_UPDATE_INTERVAL) {
    readCANMessages();
    lastCanUpdate = currentTime;
  }
  
  // Update display
  if (currentTime - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
    updateDisplayValues();
    lastDisplayUpdate = currentTime;
  }
  
  // LVGL tick and task handler
  lv_tick_inc(5);
  lv_task_handler();
  
  delay(5);
}

void initDisplay() {
  Serial.println("Initializing TFT Display...");
  
  // Initialize TFT
  tft.init();
  tft.setRotation(3); // Landscape mode
  tft.fillScreen(TFT_BLACK);
  
  // Backlight control
  pinMode(TFT_BL_PIN, OUTPUT);
  digitalWrite(TFT_BL_PIN, HIGH);
  
  Serial.println("TFT Display initialized");
}

void initLVGL() {
  Serial.println("Initializing LVGL...");
  
  lv_init();
  
  // Display buffer allocation
  static lv_color_t buf1[TFT_WIDTH * 60];
  static lv_color_t buf2[TFT_WIDTH * 60];
  static lv_disp_draw_buf_t draw_buf;
  lv_disp_draw_buf_init(&draw_buf, buf1, buf2, TFT_WIDTH * 60);
  
  // Display driver
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = TFT_WIDTH;
  disp_drv.ver_res = TFT_HEIGHT;
  disp_drv.flush_cb = displayFlush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);
  
  // Input device (optional - for touch)
  // static lv_indev_drv_t indev_drv;
  // lv_indev_drv_init(&indev_drv);
  // indev_drv.type = LV_INDEV_TYPE_POINTER;
  // indev_drv.read_cb = touchRead;
  // lv_indev_drv_register(&indev_drv);
  
  Serial.println("LVGL initialized");
}

void displayFlush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t*)&color_p->full, w * h, true);
  tft.endWrite();
  
  lv_disp_flush_ready(disp);
}

void initCAN() {
  Serial.println("Initializing TJA1051 CAN Bus...");
  
  // Configure TWAI (CAN) general configuration
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(CAN_TX_PIN, CAN_RX_PIN, TWAI_MODE_NORMAL);
  g_config.tx_queue_len = 10;
  g_config.rx_queue_len = 10;
  
  // Configure TWAI timing for 500kbps (or 1Mbps)
  twai_timing_config_t t_config = CAN_SPEED;
  
  // Configure TWAI filter to accept all messages (no filtering)
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
  
  // Install TWAI driver
  esp_err_t result = twai_driver_install(&g_config, &t_config, &f_config);
  if (result == ESP_OK) {
    Serial.println("TJA1051 CAN Bus initialized successfully");
  } else {
    Serial.printf("TJA1051 CAN Bus initialization failed: %s\n", esp_err_to_name(result));
    return;
  }
  
  // Start TWAI driver
  result = twai_start();
  if (result == ESP_OK) {
    Serial.println("TJA1051 CAN Bus started");
  } else {
    Serial.printf("TJA1051 CAN Bus start failed: %s\n", esp_err_to_name(result));
    return;
  }
  
  Serial.println("TJA1051 CAN Bus ready - listening on 500kbps");
}

void initWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection failed!");
  }
}

void readCANMessages() {
  twai_message_t rx_msg;
  
  // Check for received CAN messages (non-blocking)
  esp_err_t result = twai_receive(&rx_msg, 0);
  if (result == ESP_OK) {
    // Process received CAN message
    processCANMessage(rx_msg.identifier, rx_msg.data_length_code, rx_msg.data);
    
    Serial.printf("CAN RX: ID=0x%03lX, DLC=%d, Data=", rx_msg.identifier, rx_msg.data_length_code);
    for (int i = 0; i < rx_msg.data_length_code; i++) {
      Serial.printf("%02X ", rx_msg.data[i]);
    }
    Serial.println();
  }
  
  // Send simulated data if no real CAN data (for testing)
  static unsigned long lastSimUpdate = 0;
  if (millis() - lastSimUpdate > 1000) {
    simulateECUData();
    lastSimUpdate = millis();
  }
}

void processCANMessage(long unsigned int id, unsigned char len, unsigned char* data) {
  switch (id) {
    case TCU_CAN_ID: // TCU Data
      if (len >= 4) {
        ecuData.torqueRequest = data[0];
        ecuData.tcuProtection = (data[1] & 0x01) != 0;
        ecuData.tcuLimpMode = (data[1] & 0x02) != 0;
        Serial.printf("TCU: Torque=%d%%, Protection=%d, Limp=%d\n", 
                     ecuData.torqueRequest, ecuData.tcuProtection, ecuData.tcuLimpMode);
      }
      break;
      
    case ECU_CAN_ID: // ECU Data
      if (len >= 6) {
        ecuData.engineRpm = (data[1] << 8) | data[0];
        ecuData.mapPressure = (data[3] << 8) | data[2];
        ecuData.tpsPosition = data[4];
        Serial.printf("ECU: RPM=%d, MAP=%dkPa, TPS=%d%%\n", 
                     ecuData.engineRpm, ecuData.mapPressure, ecuData.tpsPosition);
      }
      break;
      
    case BOOST_CAN_ID: // Boost Control
      if (len >= 4) {
        ecuData.wastegatePos = data[0];
        ecuData.targetBoost = (data[2] << 8) | data[1];
        Serial.printf("Boost: Wastegate=%d%%, Target=%dkPa\n", 
                     ecuData.wastegatePos, ecuData.targetBoost);
      }
      break;
  }
}

void simulateECUData() {
  // Simulate realistic ECU data for testing
  static float rpmBase = 1000;
  static bool rpmUp = true;
  
  // Simulate RPM sweep
  if (rpmUp) {
    rpmBase += 50;
    if (rpmBase > 6000) rpmUp = false;
  } else {
    rpmBase -= 50;
    if (rpmBase < 1000) rpmUp = true;
  }
  
  ecuData.engineRpm = rpmBase + random(-100, 100);
  ecuData.mapPressure = map(ecuData.engineRpm, 1000, 6000, 100, 230) + random(-5, 5);
  ecuData.tpsPosition = map(ecuData.engineRpm, 1000, 6000, 10, 90) + random(-10, 10);
  ecuData.wastegatePos = map(ecuData.mapPressure, 100, 230, 0, 80) + random(-5, 5);
  ecuData.targetBoost = ecuData.mapPressure + random(-10, 10);
  
  // Simulate TCU protection at high RPM
  if (ecuData.engineRpm > 5500) {
    ecuData.tcuProtection = true;
  } else if (ecuData.engineRpm < 5000) {
    ecuData.tcuProtection = false;
  }
  
  // Simulate limp mode at very high RPM
  ecuData.tcuLimpMode = (ecuData.engineRpm > 6200);
  
  // Constrain values
  ecuData.mapPressure = constrain(ecuData.mapPressure, 100, 250);
  ecuData.tpsPosition = constrain(ecuData.tpsPosition, 0, 100);
  ecuData.wastegatePos = constrain(ecuData.wastegatePos, 0, 100);
  ecuData.targetBoost = constrain(ecuData.targetBoost, 100, 250);
  ecuData.engineRpm = constrain(ecuData.engineRpm, 0, 7000);
}

void updateDisplayValues() {
  // Update MAP Pressure gauge
  ui_update_map_pressure(ecuData.mapPressure);
  
  // Update Wastegate gauge
  ui_update_wastegate_position(ecuData.wastegatePos);
  
  // Update TPS gauge
  ui_update_tps_position(ecuData.tpsPosition);
  
  // Update RPM gauge (with warning colors)
  ui_update_engine_rpm(ecuData.engineRpm);
  
  // Update Target Boost gauge
  ui_update_target_boost(ecuData.targetBoost);
  
  // Update TCU Status
  ui_update_tcu_status(ecuData.tcuProtection, ecuData.tcuLimpMode);
  
  // Update connection status - check TWAI driver status
  twai_status_info_t status_info;
  esp_err_t status_result = twai_get_status_info(&status_info);
  bool canConnected = (status_result == ESP_OK && status_info.state == TWAI_STATE_RUNNING);
  ui_update_connection_status(canConnected);
  
  // Debug output
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 2000) {
    Serial.printf("Data: MAP=%dkPa, WG=%d%%, TPS=%d%%, RPM=%d, Target=%dkPa\n",
                 ecuData.mapPressure, ecuData.wastegatePos, ecuData.tpsPosition,
                 ecuData.engineRpm, ecuData.targetBoost);
    lastDebug = millis();
  }
}

void sendCANMessage(uint16_t id, uint8_t* data, uint8_t len) {
  twai_message_t tx_msg;
  tx_msg.identifier = id;
  tx_msg.data_length_code = len;
  tx_msg.flags = TWAI_MSG_FLAG_NONE;  // Standard frame
  
  for (int i = 0; i < len && i < 8; i++) {
    tx_msg.data[i] = data[i];
  }
  
  esp_err_t result = twai_transmit(&tx_msg, pdMS_TO_TICKS(1000));
  if (result == ESP_OK) {
    Serial.printf("CAN TX: ID=0x%03X sent successfully\n", id);
  } else {
    Serial.printf("CAN TX failed: %s\n", esp_err_to_name(result));
  }
}

// Touch input handler (optional)
void touchRead(lv_indev_drv_t * indev_driver, lv_indev_data_t * data) {
  // Implement touch reading if using touch display
  data->state = LV_INDEV_STATE_REL;
  data->point.x = 0;
  data->point.y = 0;
}