#include "render/shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace {

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Не удалось открыть файл: " << path << '\n';
        return {};
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

GLuint compile(GLenum type, const std::string& source, const std::string& name) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint ok = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        std::cerr << "Ошибка компиляции шейдера " << name << ":\n" << log << '\n';
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

} // namespace

GLuint loadShaderProgram(const std::string& vertPath, const std::string& fragPath) {
    const std::string vertSrc = readFile(vertPath);
    const std::string fragSrc = readFile(fragPath);
    if (vertSrc.empty() || fragSrc.empty()) return 0;

    GLuint vs = compile(GL_VERTEX_SHADER, vertSrc, vertPath);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fragSrc, fragPath);
    if (!vs || !fs) return 0;

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint ok = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        std::cerr << "Ошибка линковки программы:\n" << log << '\n';
        glDeleteProgram(program);
        return 0;
    }
    return program;
}