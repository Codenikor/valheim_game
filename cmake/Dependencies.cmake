include(FetchContent)

# SDL3 — окно, ввод, контекст OpenGL.
# Версия зафиксирована тегом: на обоих ноутбуках будет ровно она.
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.14
    GIT_SHALLOW    TRUE
)

# Собираем SDL статически и без лишнего (тесты, примеры)
set(SDL_SHARED  OFF CACHE BOOL "" FORCE)
set(SDL_STATIC  ON  CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL3)