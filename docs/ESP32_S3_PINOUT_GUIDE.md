# ESP32-S3-Touch-LCD-7 Распиновка и Подключение

## 🖥️ Экран ST7262 RGB LCD (800x480)

### RGB Интерфейс (16-bit данных):
```
DATA PINS:
GPIO8  → LCD_D0   (Red 0)
GPIO3  → LCD_D1   (Red 1) 
GPIO46 → LCD_D2   (Red 2)
GPIO9  → LCD_D3   (Red 3)
GPIO1  → LCD_D4   (Red 4)

GPIO5  → LCD_D5   (Green 0)
GPIO6  → LCD_D6   (Green 1)
GPIO7  → LCD_D7   (Green 2)
GPIO15 → LCD_D8   (Green 3)
GPIO16 → LCD_D9   (Green 4)
GPIO4  → LCD_D10  (Green 5)

GPIO45 → LCD_D11  (Blue 0)
GPIO48 → LCD_D12  (Blue 1)
GPIO47 → LCD_D13  (Blue 2)
GPIO21 → LCD_D14  (Blue 3)
GPIO14 → LCD_D15  (Blue 4)
```

### Управляющие сигналы:
```
GPIO40 → LCD_CLK    (Pixel Clock)
GPIO41 → LCD_CAM_D+ (HSYNC)
GPIO39 → LCD_CAM_D- (VSYNC)  
GPIO42 → LCD_DISP   (Display Enable)
```

### Питание и подсветка:
```
3.3V   → LCD_VDD
GND    → LCD_GND
GPIO2  → LCD_BLK (Backlight Control)
```

---

## 👆 Тачскрин GT911 (I2C)

### I2C интерфейс:
```
GPIO19 → GT911_SDA  (I2C Data)
GPIO20 → GT911_SCL  (I2C Clock)
GPIO38 → GT911_INT  (Interrupt)
GPIO18 → GT911_RST  (Reset)
```

### Питание:
```
3.3V → GT911_VDD
GND  → GT911_GND
```

### I2C адрес:
- **0x5D** (по умолчанию)
- **0x14** (альтернативный)

---

## 🚗 CAN Шина TJA1051

### TWAI (CAN) интерфейс:
```
GPIO17 → CAN_TX  (Transmit)
GPIO18 → CAN_RX  (Receive)  
```

### Питание и сигналы:
```
5V     → TJA1051_VCC
GND    → TJA1051_GND
CAN_H  → CAN Bus High
CAN_L  → CAN Bus Low
```

### Терминирующие резисторы:
- **120 Ом** между CAN_H и CAN_L на концах шины

---

## 🔌 GPIO Расширитель CH422G (I2C)

### I2C интерфейс:
```
GPIO19 → CH422G_SDA  (Shared with GT911)
GPIO20 → CH422G_SCL  (Shared with GT911)
```

### I2C адрес:
- **0x24** (по умолчанию)

### Расширенные GPIO:
```
CH422G_IO0 → User GPIO 0
CH422G_IO1 → User GPIO 1
CH422G_IO2 → User GPIO 2
CH422G_IO3 → User GPIO 3
CH422G_IO4 → User GPIO 4
CH422G_IO5 → User GPIO 5
CH422G_IO6 → User GPIO 6
CH422G_IO7 → User GPIO 7
```

---

## 🔗 USB-UART CH343

### UART интерфейс для программирования:
```
GPIO43 → CH343_TX  (UART0 TX)
GPIO44 → CH343_RX  (UART0 RX)
```

### Boot режим:
```
GPIO0  → BOOT Button (Pull-down for download mode)
EN     → Reset Button
```

---

## ⚡ Питание ESP32-S3

### Основное питание:
```
5V Input → USB-C или внешний источник
3.3V     → Внутренний регулятор
GND      → Общая земля
```

### PSRAM и Flash:
```
16MB Flash  → Встроенная (QSPI)
8MB PSRAM   → Встроенная (QSPI)
```

---

## 📋 Конфигурация ESP-IDF

### sdkconfig важные параметры:

```ini
# LCD Configuration
CONFIG_LCD_RGB_ISR_IRAM_SAFE=y
CONFIG_LCD_RGB_RESTART_IN_VSYNC=y

# PSRAM Configuration  
CONFIG_SPIRAM=y
CONFIG_SPIRAM_MODE_OCT=y
CONFIG_SPIRAM_SPEED_80M=y

# TWAI (CAN) Configuration
CONFIG_TWAI_ISR_IN_IRAM=y

# I2C Configuration
CONFIG_I2C_ENABLE_DEBUG_LOG=y

# FreeRTOS Configuration
CONFIG_FREERTOS_TICK_RATE_HZ=1000
CONFIG_FREERTOS_USE_TRACE_FACILITY=y
```

---

## 🔧 Инициализация в коде

### LCD RGB инициализация:
```c
esp_lcd_rgb_panel_config_t panel_config = {
    .data_width = 16,
    .bits_per_pixel = 16,
    .de_gpio_num = 42,
    .pclk_gpio_num = 40,
    .vsync_gpio_num = 39,
    .hsync_gpio_num = 41,
    .data_gpio_nums = {
        8, 3, 46, 9, 1,     // R0-R4
        5, 6, 7, 15, 16, 4, // G0-G5  
        45, 48, 47, 21, 14  // B0-B4
    },
    .timings = {
        .pclk_hz = 16000000,
        .h_res = 800,
        .v_res = 480,
        .hsync_front_porch = 8,
        .hsync_back_porch = 8,
        .hsync_pulse_width = 4,
        .vsync_front_porch = 8,
        .vsync_back_porch = 8,
        .vsync_pulse_width = 4,
    }
};
```

### GT911 Touch инициализация:
```c
i2c_config_t i2c_config = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = 19,
    .scl_io_num = 20,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = 400000
};
```

### TWAI (CAN) инициализация:
```c
twai_general_config_t g_config = {
    .mode = TWAI_MODE_NORMAL,
    .tx_io = 17,
    .rx_io = 18,
    .clkout_io = TWAI_IO_UNUSED,
    .bus_off_io = TWAI_IO_UNUSED,
    .tx_queue_len = 10,
    .rx_queue_len = 10,
    .alerts_enabled = TWAI_ALERT_NONE,
    .clkout_divider = 0
};

twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
```

---

## ⚠️ Важные замечания

### GPIO ограничения ESP32-S3:
- **GPIO26-32:** Зарезервированы для SPI Flash/PSRAM
- **GPIO33-37:** Зарезервированы для SPI Flash/PSRAM  
- **GPIO19-20:** Используются для I2C (GT911 + CH422G)
- **GPIO43-44:** UART0 для программирования

### Уровни напряжения:
- **ESP32-S3 GPIO:** 3.3V logic
- **ST7262 LCD:** 3.3V logic  
- **GT911 Touch:** 3.3V logic
- **TJA1051 CAN:** 5V tolerant, 3.3V logic
- **CH422G:** 3.3V logic

### Максимальные частоты:
- **LCD Pixel Clock:** до 20MHz
- **I2C:** до 400kHz (Fast Mode)
- **CAN:** до 1Mbps
- **SPI Flash:** до 80MHz (Octal)

---

**Файл:** ESP32_S3_PINOUT_GUIDE.md  
**Версия:** 1.0  
**Дата:** 30 июля 2025  
**Совместимость:** ESP32-S3-Touch-LCD-7, ESP-IDF v5.3+