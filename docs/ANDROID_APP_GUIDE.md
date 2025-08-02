# ECU Dashboard Android App - Руководство по использованию

## 🎯 Обзор

Android приложение для приема и отображения CAN данных от ESP32 ECU Dashboard через WiFi или Bluetooth соединение.

### Возможности:
- ✓ **WiFi подключение** через WebSocket
- ✓ **Bluetooth подключение** (в разработке)
- ✓ **Реальное время** обновление данных (10Hz)
- ✓ **6 анимированных датчиков** как в основном дашборде
- ✓ **PWA поддержка** - установка как приложение
- ✓ **Адаптивный дизайн** для телефонов и планшетов

---

## 📱 Способы использования

### Вариант 1: Native Android App (ГОТОВО!)

**Полноценное нативное приложение создано:**
- ✓ Kotlin + Jetpack Compose UI
- ✓ WiFi подключение через WebSocket
- ✓ 6 анимированных датчиков
- ✓ Material 3 Design
- ✓ Настройки и демо режим

**Как собрать и установить:**
1. Откройте `ECU_Dashboard_Android/` в Android Studio
2. Синхронизируйте Gradle файлы
3. Соберите APK: `Build → Build Bundle(s) / APK(s) → Build APK(s)`
4. Установите APK на Android устройство

### Вариант 2: Web App (Альтернатива)

**Для быстрого тестирования:**
1. Подключитесь к WiFi сети ESP32:
   - SSID: `ECU_Dashboard`
   - Password: `12345678`

2. Откройте Chrome на Android:
   - Перейдите на: `http://192.168.4.1/mobile`

3. Установите как приложение (опционально):
   - Нажмите меню (⋮) → "Добавить на главный экран"

---

## 🔧 Настройка ESP32 для WebSocket

### Обновление main.c:

```c
#include "can_websocket.h"

void app_main(void)
{
    // ... существующая инициализация ...
    
    // Запуск WiFi AP
    wifi_init_softap();
    
    // Запуск WebSocket сервера
    start_websocket_server();
    
    // Создание задачи broadcast
    xTaskCreate(websocket_broadcast_task, "WS_BROADCAST", 4096, NULL, 5, NULL);
    
    // ... остальной код ...
}

// В CAN обработчике добавить:
void can_receive_task(void *pvParameters)
{
    // ... получение CAN данных ...
    
    // Обновление WebSocket данных
    update_websocket_can_data(
        engine_rpm, 
        map_pressure, 
        tps_position,
        wastegate_pos,
        target_boost,
        tcu_status
    );
}
```

### WiFi конфигурация:

```c
void wifi_init_softap(void)
{
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = "ECU_Dashboard",
            .ssid_len = strlen("ECU_Dashboard"),
            .channel = 1,
            .password = "12345678",
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_LOGI(TAG, "WiFi AP started. SSID:%s password:%s", 
             "ECU_Dashboard", "12345678");
}
```

---

## 📊 Протокол передачи данных

### WebSocket JSON формат:

```json
{
  "map_pressure": 125,      // 100-250 kPa
  "wastegate_pos": 45,      // 0-100 %
  "tps_position": 30,       // 0-100 %
  "engine_rpm": 3500,       // 0-7000 RPM
  "target_boost": 180,      // 100-250 kPa
  "tcu_status": 0           // 0=OK, 1=WARN, 2=ERROR
}
```

### Частота обновления:
- **WebSocket broadcast:** 10Hz (каждые 100ms)
- **UI обновление:** 60fps с плавной анимацией
- **CAN polling:** максимально быстро

---

## 🎨 Интерфейс приложения

### Датчики:
1. **MAP Pressure** (Синий #00D4FF)
   - Диапазон: 100-250 kPa
   - Предупреждение: >200 kPa
   - Опасность: >230 kPa

2. **Wastegate** (Зеленый #00FF88)
   - Диапазон: 0-100%
   - Положение актуатора

3. **TPS** (Желтый #FFD700)
   - Диапазон: 0-100%
   - Положение дроссельной заслонки

4. **Engine RPM** (Оранжевый #FF6B35)
   - Диапазон: 0-7000 RPM
   - Предупреждение: >5500 RPM
   - Опасность: >6500 RPM

5. **Target Boost** (Желтый #FFD700)
   - Диапазон: 100-250 kPa
   - Целевое давление наддува

6. **TCU Status** (LED индикатор)
   - Зеленый: OK
   - Желтый: WARNING
   - Красный (мигает): ERROR

---

## 🔌 Bluetooth подключение (В разработке)

### Планируемая реализация:

```javascript
// Bluetooth Web API для Chrome Android
navigator.bluetooth.requestDevice({
  filters: [{ name: 'ECU_Dashboard' }],
  optionalServices: ['12345678-1234-5678-1234-56789abcdef0']
})
.then(device => device.gatt.connect())
.then(server => server.getPrimaryService('12345678-1234-5678-1234-56789abcdef0'))
.then(service => service.getCharacteristic('87654321-4321-8765-4321-ba9876543210'))
.then(characteristic => {
  characteristic.addEventListener('characteristicvaluechanged', handleData);
  return characteristic.startNotifications();
});
```

---

## 📋 Проверочный список

### ESP32 сторона:
```
☐ WiFi AP настроен и работает
☐ WebSocket сервер запущен на порту 81
☐ CAN данные обновляются в реальном времени
☐ JSON формат соответствует спецификации
```

### Android сторона:
```
☐ Подключение к WiFi ESP32
☐ Открытие веб-приложения
☐ Все 6 датчиков отображаются
☐ Данные обновляются плавно
☐ TCU статус индикация работает
```

---

## 🚀 Расширенные возможности

### Будущие улучшения:
1. **Запись данных** на SD карту
2. **Графики истории** параметров
3. **Настройки тревог** и уведомлений
4. **OBD-II** диагностические коды
5. **Голосовые** предупреждения
6. **GPS интеграция** для треков

### Нативное Android приложение:
- Kotlin + Jetpack Compose UI
- Room database для истории
- WorkManager для фоновой записи
- Firebase для облачного backup

---

## 🔍 Отладка

### WebSocket проверка:
```bash
# Проверка подключения
curl -i -N -H "Connection: Upgrade" -H "Upgrade: websocket" \
     -H "Sec-WebSocket-Version: 13" \
     -H "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==" \
     http://192.168.4.1:81/ws

# Chrome DevTools
# Console: new WebSocket('ws://192.168.4.1:81/ws')
```

### ESP32 логи:
```
I (12345) CAN_WEBSOCKET: Starting WebSocket server on port: '81'
I (12346) CAN_WEBSOCKET: WebSocket server started
I (12500) CAN_WEBSOCKET: Handshake done, new WebSocket connection opened
I (12600) CAN_WEBSOCKET: Broadcasting CAN data to 1 clients
```

---

**Файл:** ANDROID_APP_GUIDE.md  
**Версия:** 1.0  
**Дата:** 26 июля 2025  
**Совместимость:** ESP32-S3, Android 5.0+, Chrome 89+