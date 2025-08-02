# ECU Dashboard Migration to EEZ Studio

## 🎯 Обзор миграции

Этот гайд поможет адаптировать проект ECU Dashboard с SquareLine Studio на **EEZ Studio** для более продвинутого UI развития и расширенной функциональности.

### Преимущества EEZ Studio для ECU Dashboard:
- ✓ **Flow-based programming** для сложной логики CAN-шины
- ✓ **Expression builder** для расчета параметров датчиков
- ✓ **Timeline анимации** для плавных переходов значений
- ✓ **Multi-screen поддержка** для расширения интерфейса
- ✓ **Built-in отладчик** с breakpoints
- ✓ **Open source** - полностью бесплатно

---

## 📋 Подготовка к миграции

### Шаг 1: Установка EEZ Studio

**Windows/macOS/Linux:**
1. Скачайте EEZ Studio с официального сайта: https://www.envox.eu/studio/
2. Или GitHub releases: https://github.com/eez-open/studio/releases
3. Установите и запустите EEZ Studio

### Шаг 2: Анализ текущей структуры проекта

**Текущие файлы SquareLine:**
```
main/ui/
├── ui.c                    # Основной UI файл
├── ui.h                    # Заголовочные файлы
├── ui_helpers.c            # Вспомогательные функции
├── ui_helpers.h            # Заголовки помощников
├── ui_events.h             # Обработчики событий
└── screens/
    ├── ui_Screen1.c        # Главный экран с датчиками
    └── ui_Screen1.h        # Заголовочный файл
```

**Что нужно адаптировать:**
- 6 круговых датчиков (MAP, Wastegate, TPS, RPM, Target Boost, TCU Status)
- Цветовые схемы для каждого датчика
- Анимации изменения значений
- CAN-данные интеграция

---

## 🛠️ Создание проекта в EEZ Studio

### Шаг 3: Создание LVGL проекта в EEZ Studio

**ВАЖНО: EEZ Studio НЕ поддерживает импорт JSON! Все настраивается через визуальный интерфейс.**

1. **Запуск EEZ Studio:**
   - Загрузите с: https://github.com/eez-open/studio/releases
   - Установите и запустите EEZ Studio

2. **Создание проекта:**
   - Нажмите кнопку **"Create"** на главной странице
   - Выберите **"LVGL"** template (НЕ "LVGL with EEZ Flow")
   - Project name: **"ECU_Dashboard"**
   - Выберите папку проекта
   - Нажмите **"Create Project"**

3. **Настройка дисплея:**
   - Откройте **Pages → main**
   - В **Properties** панели (справа) установите:
     ```
     Width: 800
     Height: 480
     ```
   - Перейдите в **Settings** и подтвердите те же размеры

### Шаг 4: Визуальное создание UI компонентов

**Интерфейс EEZ Studio:**
- **Левая панель:** Pages/Widgets/Variables
- **Центр:** Визуальный редактор (canvas)
- **Правая панель:** Properties + Components palette  
- **Toolbar:** Build button (🔧) для генерации кода

**Создание экрана с датчиками (drag-and-drop):**

### Шаг 5: Создание датчиков через drag-and-drop

**1. MAP Pressure Gauge (Синий):**
```
1. Перетащите "Arc" из Components palette на canvas
2. Установите Position: X=50, Y=50, Width=180, Height=180
3. В Properties настройте:
   - Min: 100
   - Max: 250  
   - Value: создайте переменную "map_pressure"
   - Color: #00D4FF
   - Background: #2d2d2d
4. Добавьте Label с текстом "MAP kPa"
```

**2. Wastegate Position (Зеленый):**
```
1. Drag "Arc" → Position: X=280, Y=50, Size=180x180
2. Properties: Min=0, Max=100, Color=#00FF88
3. Variable: "wastegate_pos"
4. Label: "Wastegate %"
```

**3. TPS Position (Желтый):**
```
1. Drag "Arc" → Position: X=510, Y=50, Size=180x180  
2. Properties: Min=0, Max=100, Color=#FFD700
3. Variable: "tps_position"
4. Label: "TPS %"
```

**4. Engine RPM (Оранжевый):**
```
1. Drag "Arc" → Position: X=50, Y=280, Size=180x180
2. Properties: Min=0, Max=7000, Color=#FF6B35
3. Variable: "engine_rpm"
4. Label: "RPM"
5. Добавьте красную зону для > 6000 RPM
```

**5. Target Boost (Желтый):**
```
1. Drag "Arc" → Position: X=280, Y=280, Size=180x180
2. Properties: Min=100, Max=250, Color=#FFD700  
3. Variable: "target_boost"
4. Label: "Target kPa"
```

**6. TCU Status LED:**
```
1. Drag "LED" → Position: X=570, Y=350, Size=120x80
2. Properties: добавьте 3 состояния:
   - State 0: Green, "TCU OK"
   - State 1: Yellow, "TCU WARN"  
   - State 2: Red, "TCU ERROR"
3. Variable: "tcu_status"
```

---

## 📊 Настройка переменных и данных

### Шаг 6: Создание переменных в EEZ Studio

**В левой панели Variables:**
1. Нажмите **"+"** для добавления переменной
2. Создайте каждую переменную:

```cpp
Название: map_pressure
Тип: integer
Диапазон: 100-250
Начальное: 100

Название: wastegate_pos  
Тип: integer
Диапазон: 0-100
Начальное: 0

Название: tps_position
Тип: integer
Диапазон: 0-100  
Начальное: 0

Название: engine_rpm
Тип: integer
Диапазон: 0-7000
Начальное: 0

Название: target_boost
Тип: integer
Диапазон: 100-250
Начальное: 100

Название: tcu_status
Тип: integer
Диапазон: 0-2
Начальное: 0
```

### Шаг 7: Flow-based логика

**1. CAN Data Reception Flow:**
```
[CAN Message Received] → [Parse Data] → [Update Variables] → [Trigger Animation]
```

**2. Data Validation Flow:**
```
[Receive Value] → [Range Check] → [Error Handling] → [Display Update]
```

**3. Animation Flow:**
```
[Value Change] → [Calculate Delta] → [Smooth Transition] → [Update Display]
```

---

## 🔧 Генерация и интеграция кода

### Шаг 8: Генерация C++ кода

1. **Сохраните проект:** Ctrl+S
2. **Build project:** Нажмите кнопку 🔧 (wrench) в toolbar
3. **Проверьте генерацию:** В папке проекта появится:
   ```
   Generated files:
   ├── ui.h/c              # Основной UI файл
   ├── screens.h/c         # Экраны и компоненты  
   ├── vars.h/c            # Переменные проекта
   ├── actions.h/c         # Обработчики событий
   └── assets.h/c          # Ресурсы (шрифты)
   ```

### Шаг 9: Интеграция с ESP32-S3

**Обновление main.c:**
```c
#include "ui.h"              // EEZ Studio UI
#include "vars.h"            // Переменные EEZ

// В main() после инициализации LVGL:
void app_main(void)
{
    // ... LCD и LVGL инициализация ...
    
    // Инициализация EEZ Studio UI
    ui_init();
    
    // Создание задач
    xTaskCreatePinnedToCore(can_receive_task, "CAN_RX", 4096, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(ui_update_task, "UI_UPDATE", 4096, NULL, 4, NULL, 0);
    
    // Основной LVGL цикл
    while (1) {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}
```

**Обновление CAN обработчика:**
```c
void update_engine_gauges(uint16_t rpm, uint16_t map, uint8_t tps)
{
    // Обновление переменных EEZ Studio
    set_var_engine_rpm(rpm);
    set_var_map_pressure(map);
    set_var_tps_position(tps);
    
    // Вызов обновления UI
    ui_update();
    
    data_received = true;
    last_can_update = xTaskGetTickCount();
}

void update_boost_gauges(uint8_t wastegate, uint16_t target_boost)
{
    set_var_wastegate_pos(wastegate);
    set_var_target_boost(target_boost);
    ui_update();
}

void update_tcu_status(uint8_t protection_flags)
{
    // Определение статуса на основе флагов
    int8_t status = 0;  // OK
    if (protection_flags & 0x20) status = 2;      // ERROR (Limp mode)
    else if (protection_flags & 0x0F) status = 1; // WARNING
    
    set_var_tcu_status(status);
    ui_update();
}
```

---

## 🎨 Расширенные возможности EEZ Studio

### Шаг 10: Добавление анимаций

**Timeline Editor для плавных переходов:**
1. Создайте Timeline для каждого датчика
2. Настройте easing functions (ease-in-out)
3. Добавьте keyframes для разных состояний:
   - Normal operation
   - Warning zone  
   - Error state
   - No data state

**Expression Builder для расчетов:**
```javascript
// MAP Pressure color calculation
if (map_pressure > 200) 
    return "#FF0000";  // Red - high pressure
else if (map_pressure > 180)
    return "#FFA500";  // Orange - warning
else 
    return "#00D4FF";  // Blue - normal

// RPM gauge color zones
if (engine_rpm > 6000)
    return "#FF0000";  // Red zone
else if (engine_rpm > 4500)
    return "#FFA500";  // Orange zone  
else
    return "#00FF00";  // Green zone
```

### Шаг 11: Multi-screen navigation

**Дополнительные экраны:**
1. **DiagnosticScreen** - детальная диагностика CAN
2. **SettingsScreen** - настройки отображения
3. **LoggingScreen** - запись данных на SD карту
4. **AlertScreen** - критические предупреждения

**Navigation flow:**
```
MainScreen ←→ DiagnosticScreen
     ↓
SettingsScreen ←→ LoggingScreen
     ↓
  AlertScreen (popup)
```

---

## 📋 Контрольный список миграции

### Pre-migration checklist:
```
☐ EEZ Studio установлена и протестирована  
☐ Текущий LVGL проект работает корректно
☐ Определена структура новых экранов
☐ Переменные и их типы задокументированы
```

### Migration steps:
```
☐ 1. Загружен и установлен EEZ Studio
☐ 2. Создан новый LVGL проект через Create button
☐ 3. Настроены размеры дисплея (800x480)
☐ 4. Добавлены 6 Arc компонентов через drag-and-drop
☐ 5. Настроены позиции, размеры и цвета каждого датчика
☐ 6. Созданы 6 переменных в Variables панели
☐ 7. Привязаны переменные к соответствующим Arc компонентам
☐ 8. Добавлены Labels с названиями датчиков
☐ 9. Сохранен проект и выполнен Build (🔧)
☐ 10. Скопированы сгенерированные файлы в ESP32 проект
☐ 11. Интегрирован код с CAN обработчиками
☐ 12. Протестирована работа с реальными данными
```

### Post-migration testing:
```
☐ Все датчики отображают корректные значения
☐ Анимации работают плавно (60Hz)
☐ Цветовые индикации срабатывают правильно  
☐ CAN сообщения обрабатываются без потерь
☐ Touch интерфейс отвечает корректно
☐ Система стабильна при длительной работе
```

---

## 🔍 Сравнение: SquareLine vs EEZ Studio

### SquareLine Studio:
- ✓ Проще в изучении
- ✓ Больше готовых виджетов
- ✓ Лучше для простых проектов
- ✗ Коммерческая лицензия
- ✗ Ограниченная кастомизация

### EEZ Studio:
- ✓ **Бесплатный и open source**
- ✓ **Flow-based программирование**  
- ✓ **Expression builder для сложной логики**
- ✓ **Встроенный отладчик**
- ✓ **Timeline анимации**
- ✓ **Multi-screen навигация**
- ✗ Более крутая кривая обучения
- ✗ Меньше готовых шаблонов

---

## 🎯 Заключение

**EEZ Studio идеально подходит для ECU Dashboard**, особенно если планируется:
- Расширение функциональности (диагностика, настройки)
- Более сложная логика обработки CAN данных  
- Профессиональный automotive интерфейс
- Долгосрочное развитие проекта

**Время миграции:** ~2-3 дня для опытного разработчика
**Результат:** Более мощный и гибкий ECU Dashboard

---

**Файл:** EEZ_STUDIO_MIGRATION_GUIDE.md  
**Версия:** 1.0  
**Дата:** 26 июля 2025  
**Совместимость:** EEZ Studio latest, ESP32-S3-Touch-LCD-7, LVGL 8.3