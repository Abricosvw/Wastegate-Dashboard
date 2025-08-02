# ECU Dashboard Project

Комплексное решение для мониторинга CAN шины автомобиля в реальном времени с поддержкой ESP32-S3-Touch-LCD-7 и Android приложения.

## 🚗 Обзор проекта

ECU Dashboard - это профессиональная система отображения параметров ECU через CAN шину, разработанная для ESP32-S3 с 7-дюймовым сенсорным экраном. Проект включает веб-интерфейс, нативное Android приложение и полную поддержку LVGL для встроенных систем.

## 📦 Компоненты проекта

### 🖥️ Веб-приложение (React + TypeScript)
- **Технологии:** React, TypeScript, TailwindCSS, Express.js
- **Функции:** 6 анимированных датчиков, WebSocket в реальном времени, настройки
- **Архитектура:** Full-stack с PostgreSQL, Drizzle ORM

### 📱 Android приложение (Kotlin + Jetpack Compose)
- **Технологии:** Kotlin, Jetpack Compose, Material 3, MVVM
- **Функции:** WiFi подключение к ESP32, 6 датчиков, настройки, демо режим
- **Архитектура:** MVVM с Coroutines и Flow

### 🔧 ESP32-S3 встроенная система (ESP-IDF + LVGL)
- **Технологии:** ESP-IDF v5.3+, LVGL 8.3, SquareLine Studio
- **Аппаратная поддержка:** ST7262 RGB LCD, GT911 Touch, TJA1051 CAN, CH422G GPIO
- **Функции:** CAN мониторинг, WiFi Access Point, WebSocket сервер

## 🎯 Мониторинг CAN параметров

### Отображаемые датчики:
1. **MAP Pressure** (100-250 kPa) - Давление во впускном коллекторе
2. **Wastegate Position** (0-100%) - Положение перепускного клапана
3. **TPS Position** (0-100%) - Положение дроссельной заслонки
4. **Engine RPM** (0-7000 RPM) - Обороты двигателя
5. **Target Boost** (100-250 kPa) - Целевое давление наддува
6. **TCU Status** (OK/WARN/ERROR) - Статус коробки передач

### CAN протокол:
- **ID 0x200:** Wastegate, Target Boost
- **ID 0x380:** RPM, MAP, TPS
- **ID 0x440:** TCU Data, Protection Status
- **Скорость:** 500 kbps CAN 2.0

## 🏗️ Архитектура системы

```
┌─────────────────┐    WiFi/WebSocket    ┌─────────────────┐
│  Android App    │◄──────────────────────►│   ESP32-S3      │
│  (Kotlin)       │                       │  (ESP-IDF)      │
└─────────────────┘                       └─────────────────┘
                                                    │
┌─────────────────┐    WiFi/WebSocket              │ TWAI CAN
│  Web Dashboard  │◄───────────────────────────────┤
│  (React)        │                                │
└─────────────────┘                                ▼
                                          ┌─────────────────┐
                                          │   CAN Bus       │
                                          │  (ECU/TCU)      │
                                          └─────────────────┘
```

## 🚀 Быстрый старт

### ESP32-S3 сборка:
```bash
cd esp_idf_s3_working/
idf.py build
idf.py -p COM13 flash monitor
```

### Веб-приложение:
```bash
npm install
npm run dev
# Открыть http://localhost:5000
```

### Android приложение:
```bash
cd ECU_Dashboard_Android/
# Открыть в Android Studio
# Build → Build APK
```

## 📋 Требования

### Аппаратные:
- **ESP32-S3-Touch-LCD-7** (Waveshare)
- **CAN шина** с TJA1051 трансивером
- **Питание:** 5V через USB-C или внешний источник

### Программные:
- **ESP-IDF v5.3+** для встроенной системы
- **Node.js 18+** для веб-приложения
- **Android Studio** для мобильного приложения
- **SquareLine Studio** для GUI дизайна (опционально)

## 🔌 Подключение

### WiFi настройки ESP32:
- **SSID:** ECU_Dashboard
- **Пароль:** 12345678
- **IP:** 192.168.4.1
- **WebSocket порт:** 81

### CAN шина:
- **Скорость:** 500 kbps
- **Терминация:** 120Ω резисторы
- **Напряжение:** 12V автомобильная сеть

## 📁 Структура проекта

```
ECU_Dashboard/
├── client/                    # React веб-приложение
├── server/                    # Express.js backend
├── shared/                    # Общие типы и схемы
├── ECU_Dashboard_Android/     # Kotlin Android приложение
├── esp_idf_s3_working/        # ESP-IDF проект для ESP32-S3
├── squareline_export/         # SquareLine Studio UI экспорт
├── attached_assets/           # Документация и схемы
├── ESP32_S3_PINOUT_GUIDE.md   # Распиновка ESP32-S3
├── ESP32_S3_WIRING_DIAGRAM.md # Схемы подключения
├── ANDROID_APP_GUIDE.md       # Руководство Android приложения
└── README.md                  # Этот файл
```

## 🎨 Дизайн и UI

### Цветовая схема (автомобильная тема):
- **MAP Pressure:** Синий #00D4FF
- **Wastegate:** Зеленый #00FF88
- **TPS:** Желтый #FFD700
- **Engine RPM:** Оранжевый #FF6B35
- **Target Boost:** Желтый #FFD700
- **TCU Status:** Динамическая (зеленый/желтый/красный)

### Поддерживаемые размеры экранов:
- **ESP32-S3:** 800x480 (7 дюймов)
- **Android:** Адаптивный дизайн
- **Web:** Responsive для всех устройств

## 📖 Документация

- [📱 Android App Guide](ANDROID_APP_GUIDE.md) - Руководство по Android приложению
- [🔌 ESP32-S3 Pinout](ESP32_S3_PINOUT_GUIDE.md) - Распиновка ESP32-S3
- [📐 Wiring Diagram](ESP32_S3_WIRING_DIAGRAM.md) - Схемы подключения
- [📋 Project Details](replit.md) - Детальная техническая документация

## 🔧 Разработка

### Архив готовых сборок:
- `ECU_Dashboard_ANDROID_NATIVE_COMPLETE.tar.gz` - Полное Android приложение
- `ECU_Dashboard_Working.tar.gz` - Рабочая версия ESP32-S3

### Инструменты разработки:
- **ESP-IDF:** v5.3+ с TWAI, RGB LCD, Touch поддержкой
- **SquareLine Studio:** Визуальный дизайн LVGL GUI
- **Android Studio:** Kotlin + Jetpack Compose разработка
- **Replit:** Full-stack веб-разработка

## 🚗 Применение

### Целевые сценарии:
- **Автоспорт:** Мониторинг параметров двигателя и турбонаддува
- **Тюнинг:** Диагностика и настройка ECU
- **Кастомные сборки:** Интеграция в проекты автомобилей
- **R&D:** Разработка автомобильных систем

### Совместимость ECU:
- Любые ECU с CAN 2.0 протоколом
- Поддержка стандартных и кастомных CAN ID
- Конфигурируемые параметры и диапазоны

## 📄 Лицензия

Открытый исходный код для образовательных и некоммерческих целей.

## 🤝 Вклад в проект

Приветствуются улучшения, исправления ошибок и новые функции!

---

**Версия:** 1.0.0  
**Дата:** Июль 2025  
**Автор:** ECU Dashboard Team  
**Поддержка:** ESP32-S3-Touch-LCD-7, Android 8.0+, Modern Browsers