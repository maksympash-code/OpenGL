//
// Created by maksympash on 14.04.2026.
//

#include "shader_utils.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string LoadShaderFromFile(const std::string &filePath) {
    std::ifstream shaderFile(filePath);
    if (!shaderFile.is_open()) {
        std::cerr << "Не вдалося відкрити файл: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << shaderFile.rdbuf(); // зчитує весь вміст файлу в потік

    return buffer.str(); // повертає як std::string
}
