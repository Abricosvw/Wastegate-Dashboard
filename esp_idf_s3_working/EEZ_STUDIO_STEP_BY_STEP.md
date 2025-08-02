# EEZ Studio - Пошаговое создание ECU Dashboard

## 🎯 Полная инструкция создания проекта ECU Dashboard в EEZ Studio

**ВНИМАНИЕ:** EEZ Studio работает через визуальный интерфейс drag-and-drop, а НЕ через импорт файлов!

---

## 📥 Шаг 1: Установка EEZ Studio

### Загрузка:
- **GitHub:** https://github.com/eez-open/studio/releases
- **Официальный сайт:** https://www.envox.eu/studio/

### Установка:
- **Windows:** `EEZ-Studio-Setup-x.xx.x.exe`
- **Linux:** `eezstudio_x.xx.x_amd64.deb`
- **macOS:** Универсальная версия

---

## 🆕 Шаг 2: Создание нового LVGL проекта

### 2.1 Запуск EEZ Studio
1. Откройте EEZ Studio
2. На главной странице нажмите кнопку **"Create"**

### 2.2 Выбор шаблона
1. Выберите **"LVGL"** (НЕ "LVGL with EEZ Flow")
2. **Project name:** `ECU_Dashboard`
3. **Location:** выберите папку для проекта
4. Нажмите **"Create Project"**

### 2.3 Настройка дисплея
1. В левой панели откройте **Pages → main**
2. В правой панели **Properties** установите:
   - **Width:** `800`
   - **Height:** `480`
3. Перейдите в **Settings** и подтвердите размеры

---

## 🎨 Шаг 3: Создание переменных

### 3.1 Добавление переменных
В левой панели **Variables** нажмите **"+"** и создайте:

```
1. map_pressure
   Type: integer
   Min: 100, Max: 250
   Default: 100

2. wastegate_pos
   Type: integer  
   Min: 0, Max: 100
   Default: 0

3. tps_position
   Type: integer
   Min: 0, Max: 100
   Default: 0

4. engine_rpm
   Type: integer
   Min: 0, Max: 7000
   Default: 0

5. target_boost
   Type: integer
   Min: 100, Max: 250
   Default: 100

6. tcu_status
   Type: integer
   Min: 0, Max: 2
   Default: 0
```

---

## 🔧 Шаг 4: Создание датчиков (Drag-and-Drop)

### 4.1 MAP Pressure Gauge (Синий)
1. **Перетащите "Arc"** из Components palette на canvas
2. **Position:** X=50, Y=50
3. **Size:** Width=180, Height=180
4. **Properties:**
   - Min: `100`
   - Max: `250`
   - Value: выберите переменную `map_pressure`
   - Color: `#00D4FF`
5. **Добавьте Label:** перетащите "Label" под датчик
   - Text: `MAP kPa`

### 4.2 Wastegate Position (Зеленый)
1. **Перетащите "Arc"** на canvas
2. **Position:** X=280, Y=50
3. **Size:** 180×180
4. **Properties:**
   - Min: `0`
   - Max: `100`
   - Value: `wastegate_pos`
   - Color: `#00FF88`
5. **Label:** `Wastegate %`

### 4.3 TPS Position (Желтый)
1. **Arc** → Position: X=510, Y=50, Size=180×180
2. **Properties:**
   - Min: `0`
   - Max: `100`
   - Value: `tps_position`
   - Color: `#FFD700`
3. **Label:** `TPS %`

### 4.4 Engine RPM (Оранжевый)
1. **Arc** → Position: X=50, Y=280, Size=180×180
2. **Properties:**
   - Min: `0`
   - Max: `7000`
   - Value: `engine_rpm`
   - Color: `#FF6B35`
3. **Label:** `RPM`

### 4.5 Target Boost (Желтый)
1. **Arc** → Position: X=280, Y=280, Size=180×180
2. **Properties:**
   - Min: `100`
   - Max: `250`
   - Value: `target_boost`
   - Color: `#FFD700`
3. **Label:** `Target kPa`

### 4.6 TCU Status LED
1. **Перетащите "LED"** → Position: X=570, Y=350, Size=120×80
2. **Properties → States:** нажмите "+" для добавления состояний:
   - **State 0:** Color=`#00FF00`, Text=`TCU OK`
   - **State 1:** Color=`#FFD700`, Text=`TCU WARN`
   - **State 2:** Color=`#FF0000`, Text=`TCU ERROR`
3. **Value:** выберите переменную `tcu_status`

---

## 🎨 Шаг 5: Настройка темы

### 5.1 Установка фона
1. Выберите весь экран (main)
2. В Properties установите:
   - **Background Color:** `#1a1a1a` (темный)

### 5.2 Настройка Arc стилей
Для каждого Arc датчика:
1. **Background Arc Color:** `#2d2d2d`
2. **Arc Width:** `8`
3. **Needle Color:** `#FFFFFF` (белый)

---

## 🔨 Шаг 6: Build и экспорт кода

### 6.1 Сохранение и Build
1. **Сохранить:** Ctrl+S
2. **Build:** нажмите кнопку 🔧 (wrench) в toolbar
3. **Проверьте:** отсутствие ошибок в консоли

### 6.2 Сгенерированные файлы
В папке проекта появятся:
```
├── ui.h              # Основной UI заголовок
├── ui.c              # Реализация UI
├── screens.h         # Экраны и компоненты
├── screens.c         # Реализация экранов
├── vars.h            # Переменные
├── vars.c            # Реализация переменных
└── assets.h          # Ресурсы (шрифты)
```

---

## 🔧 Шаг 7: Интеграция с ESP32-S3

### 7.1 Копирование файлов
Скопируйте сгенерированные файлы в ESP32 проект:
```bash
cp ui.* screens.* vars.* assets.* /path/to/esp32/main/ui/
```

### 7.2 Обновление main.c
```c
#include "ui/ui.h"
#include "ui/vars.h"

void app_main(void)
{
    // ... инициализация LCD и LVGL ...
    
    // Инициализация EEZ Studio UI
    ui_init();
    
    // Запуск задач
    xTaskCreate(can_receive_task, "CAN_RX", 4096, NULL, 5, NULL);
    xTaskCreate(ui_update_task, "UI_UPDATE", 4096, NULL, 4, NULL);
    
    // Основной LVGL цикл
    while (1) {
        lv_timer_handler();
        ui_tick();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
```

### 7.3 Обновление CAN обработчиков
```c
#include "ui/vars.h"

void update_engine_gauges(uint16_t rpm, uint16_t map, uint8_t tps)
{
    // Обновление переменных EEZ Studio
    set_var_engine_rpm(rpm);
    set_var_map_pressure(map);
    set_var_tps_position(tps);
}

void update_boost_gauges(uint8_t wastegate, uint16_t target)
{
    set_var_wastegate_pos(wastegate);
    set_var_target_boost(target);
}

void update_tcu_status(uint8_t protection_flags)
{
    int status = 0;  // OK
    if (protection_flags & 0x20) status = 2;      // ERROR
    else if (protection_flags & 0x0F) status = 1; // WARNING
    
    set_var_tcu_status(status);
}
```

---

## ✅ Шаг 8: Тестирование

### 8.1 Симуляция в EEZ Studio
1. В EEZ Studio нажмите **"Run"** для симуляции
2. Проверьте отображение всех датчиков
3. Измените значения переменных и проверьте анимацию

### 8.2 Тестирование на ESP32
1. Скомпилируйте проект: `idf.py build`
2. Прошейте: `idf.py flash monitor`
3. Проверьте отображение датчиков на реальном дисплее
4. Подключите CAN-шину и проверьте обновление данных

---

## 🚨 Типичные ошибки

### Проблема: Компоненты не отображаются
**Решение:** Проверьте размеры canvas (800×480) и позиции компонентов

### Проблема: Переменные не обновляются
**Решение:** Убедитесь что вызываете `ui_tick()` в основном цикле

### Проблема: Цвета не применяются
**Решение:** Проверьте формат цветов (#RRGGBB) и настройки темы

### Проблема: Build завершается с ошибками
**Решение:** Проверьте что все переменные созданы и привязаны к компонентам

---

## 📋 Контрольный список

```
☐ EEZ Studio установлена и запускается
☐ Создан новый LVGL проект "ECU_Dashboard"
☐ Настроены размеры дисплея 800×480
☐ Созданы 6 переменных для CAN данных
☐ Добавлены 6 Arc компонентов через drag-and-drop
☐ Настроены позиции, размеры и цвета датчиков
☐ Добавлен LED компонент для TCU статуса
☐ Привязаны переменные к соответствующим компонентам
☐ Добавлены Labels с названиями датчиков
☐ Установлена темная automotive тема
☐ Проект сохранен и успешно скомпилирован (🔧)
☐ Сгенерированные файлы скопированы в ESP32 проект
☐ Обновлены CAN обработчики для работы с EEZ переменными
☐ Проект протестирован на реальном hardware
```

---

**Время создания:** ~2-3 часа для первого раза  
**Результат:** Профессиональный ECU Dashboard с EEZ Studio возможностями  
**Файл:** EEZ_STUDIO_STEP_BY_STEP.md  
**Версия:** 1.0  
**Дата:** 26 июля 2025