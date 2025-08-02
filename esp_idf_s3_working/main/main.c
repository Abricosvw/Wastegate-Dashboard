#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"

// Driver
#include "driver/i2c.h"
#include "esp_lcd_touch_gt911.h"

// UI includes
#include "ui/ui.h"

// Display driver
#include "../components/espressif__esp_lcd_touch/display.h"

static const char *TAG = "ECU_DASHBOARD";



void app_main(void)
{
    ESP_LOGI(TAG, "ECU Dashboard Starting...");
    ESP_LOGI(TAG, "Free heap: %ld bytes", esp_get_free_heap_size());
    
    /* Initialize display and UI */
    display();
}

