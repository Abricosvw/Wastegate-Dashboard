# Arduino IDE Setup для ECU Dashboard

## Установка библиотек

### Обязательные библиотеки через Library Manager:
1. **TFT_eSPI** by Bodmer - для управления дисплеем
2. **LVGL** by kisvegabor - графическая библиотека

### Встроенные библиотеки ESP32:
- **driver/twai.h** - встроенный CAN драйвер ESP32 (TWAI)

### Установка через Arduino IDE:
1. Tools → Manage Libraries
2. Поиск и установка каждой библиотеки
3. Restart Arduino IDE

## Настройка TFT_eSPI

### Файл User_Setup.h:
Найдите файл в: `Arduino/libraries/TFT_eSPI/User_Setup.h`

```cpp
// Раскомментируйте нужный драйвер:
#define ILI9341_DRIVER

// Настройки пинов для ESP32:
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15
#define TFT_DC    2
#define TFT_RST   4

// Разрешение экрана:
#define TFT_WIDTH  320
#define TFT_HEIGHT 240
// Или для больших дисплеев:
// #define TFT_WIDTH  800
// #define TFT_HEIGHT 480

// SPI частота:
#define SPI_FREQUENCY  40000000
```

## Настройка LVGL

### Файл lv_conf.h:
Создайте файл `lv_conf.h` в папке проекта:

```cpp
#define LV_CONF_INCLUDE_SIMPLE
#if 1

#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 0

#define LV_MEM_CUSTOM 0
#define LV_MEM_SIZE (64U * 1024U)

#define LV_DISP_REFRESH_PERIOD 30
#define LV_INDEV_READ_PERIOD 30

#define LV_USE_PERF_MONITOR 1
#define LV_USE_MEM_MONITOR  1

#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_24 1
#define LV_FONT_MONTSERRAT_32 1

#define LV_USE_THEME_DEFAULT 1

#endif
```

## Подключение проводов

### ESP32 к TFT дисплею (ILI9341):
```
ESP32     TFT Display
3.3V  →   VCC
GND   →   GND  
GPIO23→   MOSI (SDA)
GPIO18→   SCK  (SCL)
GPIO15→   CS
GPIO2 →   DC
GPIO4 →   RST
GPIO32→   BLK (Backlight)
```

### ESP32 к TJA1051 CAN Transceiver:
```
ESP32     TJA1051
5V    →   VCC (или 3.3V в зависимости от модуля)
GND   →   GND
GPIO21→   CTX (CAN TX)
GPIO22→   CRX (CAN RX)
GND   →   S (Silent/Standby pin - заземлить для нормального режима)
```

### TJA1051 к CAN шине:
```
TJA1051   CAN Bus
CANH  →   CAN HIGH
CANL  →   CAN LOW
GND   →   CAN GND
```

### Преимущества TJA1051:
- Высокая скорость до 1 Мбит/с
- Встроенная защита от перенапряжения
- Низкое энергопотребление
- Прямое подключение к ESP32 без SPI
- Меньше компонентов и проводов

## Компиляция проекта

### Настройки Arduino IDE:
1. **Board**: ESP32 Dev Module
2. **Upload Speed**: 921600
3. **CPU Frequency**: 240MHz
4. **Flash Frequency**: 80MHz
5. **Flash Mode**: QIO
6. **Flash Size**: 4MB
7. **Partition Scheme**: Default 4MB
8. **PSRAM**: Disabled

### Файлы проекта:
```
ECU_Dashboard/
├── ECU_Dashboard.ino     (основной файл)
├── ui.h                  (из SquareLine Studio)
├── ui.c                  (из SquareLine Studio)
├── lv_conf.h            (конфигурация LVGL)
└── ecu_data_structures.h (структуры данных)
```

## Возможные ошибки

### "TFT_eSPI.h not found":
- Переустановите библиотеку TFT_eSPI
- Проверьте User_Setup.h

### "LVGL compilation error":
- Создайте lv_conf.h в папке проекта
- Установите LVGL версии 8.3.x

### "TWAI driver installation failed":
- Проверьте подключение TJA1051
- Убедитесь в правильных пинах GPIO21/GPIO22
- Проверьте питание TJA1051 (5V или 3.3V)

### "Display white screen":
- Проверьте пины подключения
- Убедитесь в правильном драйвере в User_Setup.h
- Проверьте подключение питания 3.3V

### "No CAN messages":
- Подключите 120Ω резисторы на концах CAN шины
- Проверьте скорость CAN (500 кбит/с или 1 Мбит/с)
- Убедитесь в правильном подключении CANH/CANL
- Заземлите пин S (Silent) на TJA1051
- Проверьте целостность CAN шины

## Тестирование

### Последовательный монитор (115200 baud):
```
ECU Dashboard Starting...
Initializing TFT Display...
TFT Display initialized
Initializing LVGL...
LVGL initialized
Initializing TJA1051 CAN Bus...
TJA1051 CAN Bus initialized successfully
TJA1051 CAN Bus started
TJA1051 CAN Bus ready - listening on 500kbps
ECU Dashboard Ready!
CAN RX: ID=0x380, DLC=6, Data=DC 0D A0 00 44 00
Data: MAP=160kPa, WG=45%, TPS=68%, RPM=3500, Target=180kPa
```

### Проверка дисплея:
- Черный фон
- 6 датчиков в правильных цветах
- Заголовок "Turbo Control Dashboard"
- Анимированные значения

### Проверка CAN:
- Сообщения в Serial Monitor
- Изменение значений датчиков при получении CAN данных
- Статус подключения "Connected"

После успешной компиляции и загрузки вы получите полноценную автомобильную приборную панель с поддержкой CAN-шины.