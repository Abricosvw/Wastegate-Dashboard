# Пошаговое руководство создания ECU Dashboard в SquareLine Studio

## Шаг 1: Создание нового проекта

1. Откройте SquareLine Studio
2. **Create** → **New Project**
3. Настройки проекта:
   - **Project Name**: ECU_Dashboard
   - **Resolution**: 800 × 480
   - **MCU**: ESP32S3 (или ваш микроконтроллер)
   - **Color Depth**: 16-bit
   - **Template**: Arduino ESP32

## Шаг 2: Настройка главного экрана

### Фон экрана:
1. Выберите **Screen1** в иерархии
2. **Inspector** → **Style** → **Background**
3. **Color**: `#0A0A0A` (черный)
4. **Opacity**: 255

## Шаг 3: Создание заголовка

1. **Add Widget** → **Label**
2. Настройки:
   - **Name**: HeaderLabel
   - **Text**: "Turbo Control Dashboard"
   - **Align**: Top Center
   - **Y**: 20
3. **Style**:
   - **Text Color**: `#FFFFFF`
   - **Font**: Montserrat 24 (или крупнейший доступный)

## Шаг 4: Создание статуса подключения

1. **Add Widget** → **Label**  
2. Настройки:
   - **Name**: ConnectionStatus
   - **Text**: "Disconnected"
   - **Align**: Top Right
   - **X**: -20, **Y**: 20
3. **Style**:
   - **Text Color**: `#FF3366` (красный)

## Шаг 5: Создание датчика MAP Pressure

1. **Add Widget** → **Arc**
2. **Transform**:
   - **Name**: MapGauge
   - **Width**: 200, **Height**: 200
   - **X**: -260, **Y**: 90
3. **Properties**:
   - **Value**: 150
   - **Range Min**: 100
   - **Range Max**: 250
   - **Start Angle**: 135
   - **End Angle**: 45
4. **Style**:
   - **Arc Color**: `#00D4FF` (синий)
   - **Arc Width**: 8
   - **Background Arc Width**: 8
   - **Background Arc Color**: `#333333`

### Подписи для MAP:
1. **Add Widget** → **Label** (в Arc)
   - **Text**: "MAP"
   - **Align**: Center, **Y**: -40
   - **Color**: `#FFFFFF`

2. **Add Widget** → **Label** (в Arc)
   - **Text**: "150"
   - **Align**: Center, **Y**: 0
   - **Color**: `#00D4FF`
   - **Font**: Большой размер

3. **Add Widget** → **Label** (в Arc)
   - **Text**: "kPa" 
   - **Align**: Center, **Y**: 30
   - **Color**: `#888888`

## Шаг 6: Создание датчика Wastegate

1. **Add Widget** → **Arc**
2. **Transform**:
   - **Name**: WastegateGauge
   - **Width**: 200, **Height**: 200
   - **X**: 0, **Y**: 90
3. **Properties**:
   - **Value**: 45
   - **Range**: 0-100
   - **Angles**: 135° to 45°
4. **Style**:
   - **Arc Color**: `#00FF88` (зеленый)
   - **Arc Width**: 8

### Подписи аналогично MAP:
- "Wastegate" (белый)
- "45" (зеленый)
- "%" (серый)

## Шаг 7: Создание датчика TPS

1. **Add Widget** → **Arc**
2. **Settings**:
   - **Name**: TpsGauge
   - **Position**: X: 260, Y: 90
   - **Size**: 200×200
   - **Value**: 68
   - **Range**: 0-100
   - **Color**: `#FFD700` (желтый)

### Подписи:
- "TPS" (белый)
- "68" (желтый)  
- "%" (серый)

## Шаг 8: Создание датчика RPM

1. **Add Widget** → **Arc**
2. **Settings**:
   - **Name**: RpmGauge
   - **Position**: X: -260, Y: 320
   - **Size**: 200×200
   - **Value**: 3500
   - **Range**: 0-7000
   - **Color**: `#FF6B35` (оранжевый)

### Подписи:
- "RPM" (белый)
- "3500" (оранжевый)
- "RPM" (серый)

## Шаг 9: Создание датчика Target Boost

1. **Add Widget** → **Arc**
2. **Settings**:
   - **Name**: TargetGauge
   - **Position**: X: 0, Y: 320
   - **Size**: 200×200
   - **Value**: 180
   - **Range**: 100-250
   - **Color**: `#FFD700` (желтый)

### Подписи:
- "Target" (белый)
- "180" (желтый)
- "kPa" (серый)

## Шаг 10: Создание панели TCU Status

1. **Add Widget** → **Panel**
2. **Settings**:
   - **Name**: TcuPanel
   - **Position**: X: 260, Y: 320
   - **Size**: 200×200
3. **Style**:
   - **Background**: `#1A1A1A`
   - **Border Width**: 2
   - **Border Color**: `#00FF88`
   - **Radius**: 15

### Содержимое TCU панели:
1. **Label**: "TCU Status" (белый, центр, Y: -40)
2. **Label**: "Normal" (зеленый, центр, Y: 0)  
3. **Circle** (зеленый индикатор, центр, Y: 40)

## Шаг 11: Проверка в симуляторе

1. Нажмите **Play** (▶️)
2. Проверьте:
   - ✅ Все 6 датчиков видны
   - ✅ Правильные цвета
   - ✅ Корректное расположение
   - ✅ Читаемые подписи

## Шаг 12: Экспорт проекта

1. **File** → **Export** → **Create Template Project**
2. Выберите папку для экспорта
3. Замените сгенерированные файлы на файлы из архива

## Цветовая палитра для копирования:

```
Фон экрана: #0A0A0A
Карточки: #1A1A1A
Белый текст: #FFFFFF
Серый текст: #888888
Границы: #333333

MAP: #00D4FF
Wastegate: #00FF88
TPS: #FFD700
RPM: #FF6B35
Target: #FFD700
Success: #00FF88
Warning: #FF6B35
Danger: #FF3366
```

## Позиции датчиков (сетка 3×2):

```
[MAP]     [Wastegate]  [TPS]
X:-260    X:0          X:260
Y:90      Y:90         Y:90

[RPM]     [Target]     [TCU]
X:-260    X:0          X:260  
Y:320     Y:320        Y:320
```

После создания вы увидите полноценную автомобильную приборную панель с 6 датчиками в правильных цветах и расположении!