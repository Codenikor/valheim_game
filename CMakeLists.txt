cmake_minimum_required(VERSION 3.22)
project(valheim_game LANGUAGES C CXX)

# --- Стандарт языка: C++20 без расширений компилятора ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Для clangd / автодополнения в редакторе
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Внешние зависимости (версии зафиксированы в одном месте) ---
include(cmake/Dependencies.cmake)
include(cmake/CompilerWarnings.cmake)

# --- Загрузчик OpenGL, лежит в репозитории ---
add_subdirectory(third_party/glad)

# --- Сама игра ---
add_executable(game
    src/main.cpp
    src/render/shader.cpp
)
target_include_directories(game PRIVATE src)
target_link_libraries(game PRIVATE SDL3::SDL3 glad)
set_project_warnings(game)

# Путь к ассетам — чтобы шейдеры находились при запуске из build/
target_compile_definitions(game PRIVATE
    ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets")