# valheim_game

Учебный проект: игра в духе Valheim на собственном движке.
Стек: C++20, SDL3, OpenGL 4.1 Core, CMake + Ninja. Платформы: macOS (arm64), Ubuntu (x86_64).

## Сборка

```bash
cmake --preset debug          # конфигурация (первый раз скачает SDL3, ~1–2 мин)
cmake --build --preset debug  # сборка
./build/debug/game            # запуск
```

Релизная сборка: то же самое с `release`.
Сбросить всё к чистому состоянию: `rm -rf build`.

## Зависимости

| Что  | Версия         | Как подключено                                  |
|------|----------------|-------------------------------------------------|
| SDL3 | release-3.4.14 | CMake FetchContent (`cmake/Dependencies.cmake`) |
| glad | GL 4.1 core    | сгенерирован и лежит в `third_party/glad`       |