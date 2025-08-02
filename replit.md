# ECU Dashboard Project

## Overview
The ECU Dashboard project is a web application designed to display real-time parameters from ECU CAN messages as analog gauges. It features customizable gauges with various sizes and display styles, fully integrated with the CAN bus for automotive ECU systems. The project's vision is to provide a versatile and robust solution for automotive diagnostics and monitoring, with potential applications in custom vehicle builds, racing, and automotive R&D. Its ambition is to be a leading tool for embedded automotive display development.

## User Preferences
- Язык интерфейса: Русский
- Целевая платформа: ESP32-S3-Touch-LCD-7 с ESP-IDF (приоритет)
- Hardware: ST7262 (800x480), GT911 Touch, TJA1051 CAN, CH422G GPIO
- Стиль датчиков: Автомобильная приборная панель для 7" дисплея
- Частота обновления: 60Hz (16ms) для плавной работы

## System Architecture
The project employs a robust architecture with distinct components:
-   **Frontend**: Built with React and TypeScript, featuring LVGL-like components for a rich user interface.
-   **Backend**: Powered by Node.js Express, utilizing WebSockets for real-time data communication and a REST API for configuration management.
-   **Data Layer**: PostgreSQL database with Drizzle ORM for persistent storage and data export capabilities.
-   **Communication**: WebSocket for live data feeds and REST API for configuration and control.
-   **Embedded Integration**: Full integration with SquareLine Studio for embedded system deployments, supporting customizable gauges and real-time updates.
-   **Mobile Integration**: Native Android application developed using Kotlin and Jetpack Compose, featuring MVVM architecture, animated circular gauges, TCU status indicator, and WebSocket integration for ESP32-Android communication.
-   **Hardware Adaptation**: Adapted for ESP32-S3-Touch-LCD-7 hardware, including support for ST7262 RGB LCD (800x480), GT911 Touch, TJA1051 CAN transceiver, and CH422G GPIO expander.
-   **Software Framework**: Utilizes ESP-IDF for professional development, including non-blocking TWAI CAN driver with autodiagnostics and optimized multitasking (Core 0 for GUI+Touch, Core 1 for CAN+Wi-Fi).
-   **Gauge System**:
    -   MAP Pressure: 100-250 kPa (blue #00D4FF)
    -   Wastegate Position: 0-100% (green #00FF88)
    -   TPS Position: 0-100% (yellow #FFD700)
    -   Engine RPM: 0-7000 RPM (orange #FF6B35)
    -   Target Boost: 100-250 kPa (yellow #FFD700)
    -   TCU Status: Protection/limp mode (color indication)
-   **Display Options**: Supports various gauge sizes (Small, Medium, Large, Extra Large), grid layouts (1-6 columns), and styles (circular arc, semi-arc, linear bar), with normal and compact modes.
-   **CAN Integration**: Processes data from specific CAN IDs for:
    -   TCU Data (ID 0x440): Torque request, protection status.
    -   ECU Data (ID 0x380): RPM, MAP, TPS.
    -   Boost Control (ID 0x200): Wastegate position, target boost.

## External Dependencies
-   **Frameworks/Libraries**: React, TypeScript, Node.js Express, LVGL, Drizzle ORM, Kotlin, Jetpack Compose, ESP-IDF.
-   **Databases**: PostgreSQL.
-   **Embedded GUI Tools**: SquareLine Studio.
-   **Hardware Components**: ESP32-S3-Touch-LCD-7, ST7262 RGB LCD, GT911 Touch, TJA1051 CAN Transceiver, CH422G GPIO Expander.
-   **Protocols**: WebSocket, REST API, CAN bus.
```