#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/gl.h>

#include <iostream>

#include "render/shader.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    // 1. Инициализация SDL (только видео-подсистема)
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init: " << SDL_GetError() << '\n';
        return 1;
    }

    // 2. Просим OpenGL 4.1 Core. На macOS без CORE + FORWARD_COMPATIBLE
    //    система молча выдаст старый OpenGL 2.1.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // 3. Окно. HIGH_PIXEL_DENSITY — чтобы на Retina рисовать в полном разрешении.
    SDL_Window* window = SDL_CreateWindow(
        "valheim_game", 1280, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (!window) {
        std::cerr << "SDL_CreateWindow: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "SDL_GL_CreateContext: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(1); // vsync

    // 4. Загружаем адреса функций OpenGL через glad
    if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress))) {
        std::cerr << "Не удалось загрузить OpenGL\n";
        return 1;
    }
    std::cout << "OpenGL:   " << reinterpret_cast<const char*>(glGetString(GL_VERSION)) << '\n'
              << "Renderer: " << reinterpret_cast<const char*>(glGetString(GL_RENDERER)) << '\n';

    // 5. Viewport в ПИКСЕЛЯХ (на Retina пиксели != точки окна)
    int pw = 0, ph = 0;
    SDL_GetWindowSizeInPixels(window, &pw, &ph);
    glViewport(0, 0, pw, ph);

        // 6. Треугольник: позиция (x, y) + цвет (r, g, b)
    const float vertices[] = {
        -0.5f, -0.5f,   1.0f, 0.3f, 0.2f,
         0.5f, -0.5f,   0.3f, 1.0f, 0.2f,
         0.0f,  0.5f,   0.2f, 0.4f, 1.0f,
    };

    GLuint vao = 0, vbo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(vertices)), vertices, GL_STATIC_DRAW);

    const auto stride = static_cast<GLsizei>(5 * sizeof(float));
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride,
                          reinterpret_cast<const void*>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    GLuint program = loadShaderProgram(ASSETS_DIR "/shaders/triangle.vert",
                                       ASSETS_DIR "/shaders/triangle.frag");
    if (!program) return 1;

    // 7. Главный цикл
    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) running = false;
            if (e.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
                glViewport(0, 0, e.window.data1, e.window.data2);
        }

        glClearColor(0.08f, 0.10f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    }

    // 8. Уборка в обратном порядке
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    SDL_GL_DestroyContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}