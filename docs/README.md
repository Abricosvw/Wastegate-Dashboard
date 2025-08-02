# ECU Dashboard Documentation

## 📋 Документация проекта

### 🔌 Аппаратная часть
- [ESP32-S3 Pinout Guide](ESP32_S3_PINOUT_GUIDE.md) - Полная распиновка ESP32-S3-Touch-LCD-7
- [Wiring Diagram](ESP32_S3_WIRING_DIAGRAM.md) - Схемы подключения всех компонентов

### 📱 Мобильные приложения  
- [Android App Guide](ANDROID_APP_GUIDE.md) - Руководство по Android приложению

## 🚗 Поддерживаемое оборудование

### ESP32-S3-Touch-LCD-7:
- **Дисплей:** ST7262 RGB LCD 800x480
- **Тач:** GT911 капацитивный 5-точечный
- **CAN:** TJA1051 трансивер
- **GPIO:** CH422G расширитель
- **Связь:** WiFi 2.4GHz + Bluetooth 5.0

### CAN протокол:
- **Скорость:** 500 kbps
- **ID 0x200:** Wastegate, Target Boost
- **ID 0x380:** RPM, MAP, TPS  
- **ID 0x440:** TCU Data, Protection

## 🎯 Мониторинг параметров

1. **MAP Pressure** (100-250 kPa) - Давление во впускном коллекторе
2. **Wastegate Position** (0-100%) - Положение перепускного клапана
3. **TPS Position** (0-100%) - Положение дроссельной заслонки
4. **Engine RPM** (0-7000 RPM) - Обороты двигателя
5. **Target Boost** (100-250 kPa) - Целевое давление наддува
6. **TCU Status** (OK/WARN/ERROR) - Статус коробки передач

## 🔧 Инструменты разработки

- **ESP-IDF v5.3+** - Основной фреймворк для ESP32-S3
- **SquareLine Studio** - Визуальный дизайн LVGL GUI
- **Android Studio** - Разработка Android приложения
- **Node.js 18+** - Веб-приложение и сервер

## 📊 Архитектура системы

```
Android App ←→ WiFi ←→ ESP32-S3 ←→ CAN Bus ←→ ECU/TCU
    ↑                      ↓
    └──── Web Dashboard ────┘
```

---

**Версия документации:** 1.0  
**Дата обновления:** Август 2025  
**Совместимость:** ESP32-S3-Touch-LCD-7, Android 8.0+