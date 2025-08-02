# ECU Dashboard - Структура проекта

## 📁 Обзор файлов и папок

```
ECU-Dashboard/
├── 📄 README.md                     # Основное описание проекта
├── 📄 LICENSE                       # MIT лицензия
├── 📄 .gitignore                    # Настройки Git
├── 📄 GITHUB_UPLOAD_GUIDE.md        # Инструкции загрузки на GitHub
├── 📄 PROJECT_STRUCTURE.md          # Этот файл
├── 📄 replit.md                     # Техническая документация
│
├── 📁 client/                       # React веб-приложение
│   ├── src/
│   │   ├── components/              # React компоненты
│   │   ├── pages/                   # Страницы приложения
│   │   ├── lib/                     # Утилиты и хуки
│   │   └── App.tsx                  # Главный компонент
│   ├── index.html
│   └── package.json
│
├── 📁 server/                       # Express.js backend
│   ├── routes.ts                    # API маршруты
│   ├── storage.ts                   # Интерфейс данных
│   ├── index.ts                     # Точка входа сервера
│   └── vite.ts                      # Vite integration
│
├── 📁 shared/                       # Общие TypeScript типы
│   └── schema.ts                    # Drizzle ORM схемы
│
├── 📁 ECU_Dashboard_Android/        # Android приложение (Kotlin)
│   ├── app/
│   │   ├── src/main/java/com/ecuapp/dashboard/
│   │   │   ├── MainActivity.kt      # Главная активность
│   │   │   ├── ui/screen/           # Экраны приложения
│   │   │   ├── ui/components/       # UI компоненты
│   │   │   ├── data/                # Модели данных
│   │   │   └── network/             # Сетевой слой
│   │   ├── src/main/res/            # Android ресурсы
│   │   └── build.gradle.kts         # Gradle конфигурация
│   ├── build.gradle.kts
│   └── README.md                    # Руководство Android
│
├── 📁 esp_idf_s3_working/           # ESP32-S3 проект (ESP-IDF)
│   ├── main/
│   │   ├── main.c                   # Главный файл приложения
│   │   ├── ui/                      # LVGL интерфейс
│   │   ├── can_handler.c            # CAN обработчик
│   │   └── wifi_server.c            # WiFi сервер
│   ├── components/                  # ESP-IDF компоненты
│   ├── CMakeLists.txt
│   └── sdkconfig.defaults           # Конфигурация ESP-IDF
│
├── 📁 squareline_export/            # SquareLine Studio экспорт
│   ├── ui/                          # LVGL UI файлы
│   ├── assets/                      # Графические ресурсы
│   └── export_settings.json         # Настройки экспорта
│
├── 📁 docs/                         # Документация
│   ├── README.md                    # Обзор документации
│   ├── ESP32_S3_PINOUT_GUIDE.md     # Распиновка ESP32-S3
│   ├── ESP32_S3_WIRING_DIAGRAM.md   # Схемы подключения
│   └── ANDROID_APP_GUIDE.md         # Руководство Android
│
├── 📁 releases/                     # Готовые сборки
│   └── ECU_Dashboard_ANDROID_NATIVE_COMPLETE.tar.gz
│
└── 📄 Конфигурационные файлы
    ├── package.json                 # Node.js зависимости
    ├── tsconfig.json                # TypeScript конфигурация
    ├── vite.config.ts               # Vite настройки
    ├── tailwind.config.ts           # TailwindCSS конфигурация
    ├── drizzle.config.ts            # Drizzle ORM конфигурация
    └── components.json              # shadcn/ui компоненты
```

## 🚀 Ключевые файлы для запуска

### Веб-приложение:
- `package.json` - Зависимости и скрипты
- `server/index.ts` - Запуск сервера
- `client/src/App.tsx` - React приложение

### Android приложение:
- `ECU_Dashboard_Android/app/build.gradle.kts` - Сборка
- `ECU_Dashboard_Android/app/src/main/java/.../MainActivity.kt` - Главная активность

### ESP32-S3:
- `esp_idf_s3_working/main/main.c` - Точка входа
- `esp_idf_s3_working/CMakeLists.txt` - Сборка проекта

## 📊 Статистика проекта

- **Всего файлов:** ~165
- **Размер:** ~1.6MB (без node_modules)
- **Языки:** TypeScript, Kotlin, C, HTML/CSS
- **Линии кода:** ~8000+ строк

## 🔧 Основные технологии

### Frontend:
- React 18 + TypeScript
- TailwindCSS + shadcn/ui
- Vite (сборщик)
- Wouter (роутинг)

### Backend:
- Node.js + Express
- WebSocket для real-time
- PostgreSQL + Drizzle ORM
- TypeScript

### Mobile:
- Kotlin + Jetpack Compose
- Material 3 Design
- MVVM архитектура
- WebSocket клиент

### Embedded:
- ESP-IDF v5.3+
- LVGL 8.3 GUI
- FreeRTOS multitasking
- TWAI CAN driver

## 📝 Файлы конфигурации

| Файл | Назначение |
|------|------------|
| `tsconfig.json` | TypeScript компилятор |
| `vite.config.ts` | Vite bundler и dev server |
| `tailwind.config.ts` | TailwindCSS стили |
| `drizzle.config.ts` | Database ORM |
| `components.json` | shadcn/ui компоненты |
| `.gitignore` | Git исключения |

---

**Дата создания:** Август 2025  
**Версия:** 1.0  
**Поддержка:** Все компоненты протестированы и готовы к использованию