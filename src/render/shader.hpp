#pragma once

#include <glad/gl.h>
#include <string>

// Компилирует и линкует шейдерную программу из двух файлов.
// Возвращает 0 при ошибке (текст ошибки печатается в stderr).
GLuint loadShaderProgram(const std::string& vertPath, const std::string& fragPath);