# ESP32-S3-Touch-LCD-7 Распиновка и Подключение

## 🖥️ Экран ST7262 RGB LCD (800x480) - ОБНОВЛЕНО

### RGB Интерфейс (16-bit данных):
```
// B (Blue)
GPIO14 → LCD_D0   (Blue 3)
GPIO38 → LCD_D1   (Blue 4)
GPIO18 → LCD_D2   (Blue 5)
GPIO17 → LCD_D3   (Blue 6)
GPIO10 → LCD_D4   (Blue 7)

// G (Green)
GPIO39 → LCD_D5   (Green 2)
GPIO0  → LCD_D6   (Green 3)
GPIO45 → LCD_D7   (Green 4)
GPIO48 → LCD_D8   (Green 5)
GPIO47 → LCD_D9   (Green 6)
GPIO21 → LCD_D10  (Green 7)

// R (Red)
GPIO1  → LCD_D11  (Red 3)
GPIO2  → LCD_D12  (Red 4)
GPIO42 → LCD_D13  (Red 5)
GPIO41 → LCD_D14  (Red 6)
GPIO40 → LCD_D15  (Red 7)
```

### Управляющие сигналы:
```
GPIO7  → LCD_PCLK   (Pixel Clock)
GPIO46 → HSYNC
GPIO3  → VSYNC
GPIO5  → DE         (Display Enable)
```

### Питание и подсветка:
```
3.3V   → LCD_VDD
GND    → LCD_GND
-1     → LCD_BLK (Backlight Control - не используется)
```

---

## 👆 Тачскрин GT911 (I2C) - ОБНОВЛЕНО

### I2C интерфейс:
```
GPIO8  → I2C_SDA (I2C Data)
GPIO9  → I2C_SCL (I2C Clock)
-1     → GT911_INT (Interrupt - не используется)
-1     → GT911_RST (Reset - не используется)
```

### Питание:
```
3.3V → GT911_VDD
GND  → GT911_GND
```

### I2C адрес:
- **0x5D** или **0x14** (зависит от конфигурации)

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
GPIO8 → CH422G_SDA  (Shared with GT911)
GPIO9 → CH422G_SCL  (Shared with GT911)
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

## 🔧 Инициализация в коде - ОБНОВЛЕНО

### LCD RGB инициализация:
```c
esp_lcd_rgb_panel_config_t panel_config = {
    .data_width = 16,
    .pclk_hz = 18 * 1000 * 1000,
    .h_res = 800,
    .v_res = 480,
    .hsync_gpio_num = 46,
    .vsync_gpio_num = 3,
    .de_gpio_num = 5,
    .pclk_gpio_num = 7,
    .data_gpio_nums = {
        14, 38, 18, 17, 10, // B3-B7
        39, 0, 45, 48, 47, 21, // G2-G7
        1, 2, 42, 41, 40,    // R3-R7
    },
    // ... другие параметры ...
};
```

### GT911 Touch инициализация (через общую I2C):
```c
i2c_config_t i2c_conf = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = 8,
    .scl_io_num = 9,
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = 400000,
};
i2c_param_config(I2C_MASTER_NUM, &i2c_conf);
i2c_driver_install(I2C_MASTER_NUM, i2c_conf.mode, 0, 0, 0);
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