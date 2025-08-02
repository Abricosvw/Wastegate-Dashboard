# ECU Dashboard Android App

Нативное Android приложение для приема и отображения CAN сообщений от ESP32 через WiFi или Bluetooth.

## 📱 Возможности

- **WiFi подключение** через WebSocket к ESP32
- **6 анимированных датчиков** в реальном времени
- **Автомобильная цветовая схема** с предупреждениями
- **Настройки соединения** и отображения
- **Демо режим** для тестирования без ESP32

## 🏗️ Архитектура

- **Kotlin + Jetpack Compose** - современный UI
- **MVVM Architecture** - разделение логики
- **Coroutines + Flow** - асинхронные операции
- **Material 3 Design** - современный дизайн

## 🎯 CAN Данные

Приложение получает и отображает:

1. **MAP Pressure** (100-250 kPa) - синий
2. **Wastegate Position** (0-100%) - зеленый
3. **TPS Position** (0-100%) - желтый
4. **Engine RPM** (0-7000 RPM) - оранжевый
5. **Target Boost** (100-250 kPa) - желтый
6. **TCU Status** (OK/WARN/ERROR) - цветовая индикация

## 🔧 Компиляция

### Требования:
- Android Studio Hedgehog (2023.3.1) или новее
- Android SDK 26+ (API 26)
- Kotlin 1.9.10+

### Шаги:
1. Откройте проект в Android Studio
2. Синхронизируйте Gradle файлы
3. Выберите устройство/эмулятор
4. Запустите проект (Run 'app')

## 📡 Подключение к ESP32

### WiFi настройки:
- **IP адрес:** 192.168.4.1 (по умолчанию)
- **Порт WebSocket:** 81
- **SSID ESP32:** ECU_Dashboard
- **Пароль:** 12345678

### Протокол данных:
```json
{
  "map_pressure": 125,
  "wastegate_pos": 45,
  "tps_position": 30,
  "engine_rpm": 3500,
  "target_boost": 180,
  "tcu_status": 0
}
```

## 🎨 Интерфейс

### Экраны:
1. **Connection Screen** - выбор и настройка подключения
2. **Dashboard Screen** - отображение датчиков
3. **Settings Screen** - конфигурация приложения

### Компоненты:
- **CircularGauge** - круговые датчики с анимацией
- **TCUStatusIndicator** - LED индикатор статуса
- **ConnectionStatusCard** - информация о соединении

## 🔌 Сетевая интеграция

### ESP32Client.kt:
- WebSocket подключение
- Парсинг JSON данных
- Обработка ошибок соединения
- Симуляция данных для тестирования

### ViewModels:
- **ConnectionViewModel** - управление подключением
- **DashboardViewModel** - обработка CAN данных

## 📋 Разрешения

Приложение требует:
- `INTERNET` - сетевое подключение
- `ACCESS_WIFI_STATE` - состояние WiFi
- `BLUETOOTH` - для будущей BLE поддержки
- `ACCESS_FINE_LOCATION` - для BLE сканирования

## 🚀 Сборка APK

```bash
# Debug версия
./gradlew assembleDebug

# Release версия (требует подписания)
./gradlew assembleRelease
```

## 🔍 Отладка

### Логи:
- WebSocket соединение
- Парсинг CAN данных
- Ошибки сети

### Демо режим:
Используйте "Demo Mode" для тестирования интерфейса без ESP32.

## 📈 Будущие улучшения

- **Bluetooth Low Energy** подключение
- **Запись данных** на устройство
- **Графики истории** параметров
- **Push уведомления** при критических значениях
- **Экспорт данных** в файлы

---

**Версия:** 1.0.0  
**Совместимость:** Android 8.0+ (API 26+)  
**Язык:** Kotlin  
**UI:** Jetpack Compose + Material 3