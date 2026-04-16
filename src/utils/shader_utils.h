//
// Created by maksympash on 14.04.2026.
//

#ifndef OPENGL_SHADER_UTILS_H
#define OPENGL_SHADER_UTILS_H
#include <string>

#include "glad/glad.h"


std::string LoadShaderFromFile(const std::string &filePath);
GLuint createShader(std::string &filePath, GLuint shaderType);
GLuint createProgram(
    std::string &vertexFileName,
    std::string &fragmentFileName);


#endif //OPENGL_SHADER_UTILS_H
