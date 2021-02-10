#include <stdio.h>
/*
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
//#include <glad/include/glad/glad.h>
#include <unordered_map>
#include "common.cpp"

static std::unordered_map<std::string, GLint> uniformLocationCache;

std::string* load_shader_source(std::string fileName) {
    std::ifstream ifs(fileName);
    std::string* content = new std::string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    return content;
}

void compile_shader(GLuint shader, const char* source) {
    glcall(glShaderSource(shader, 1, &source, NULL));
    glcall(glCompileShader(shader));

    int success;
    char infoLog[512];
    glcall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Shader compilation failed!" << infoLog << std::endl;
    }
}

void compile_program(GLuint program, GLuint vShader, GLuint fShader) {
    glcall(glAttachShader(program, vShader));
    glcall(glAttachShader(program, fShader));
    glcall(glLinkProgram(program));

    int success;
    char infoLog[512];

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "Program compilation failed!" << infoLog << std::endl;
    }

    glcall(glDeleteShader(vShader));
    glcall(glDeleteShader(fShader));
}

int GetUniformLocation(unsigned int program, const char* un_name) {
    int location;

    if (uniformLocationCache.find(un_name) != uniformLocationCache.end()) {
        return uniformLocationCache[un_name];
    }

    glcall(location = glGetUniformLocation(program, un_name));
    uniformLocationCache[un_name] = location;

    if (location == -1) {
        std::cout << "Warning: uniform " << un_name << " doesn't exist!" << std::endl;
    }
    return location;
}

void SetUniformMat4f(unsigned int program, const char* un_name, glm::mat4& matrix) {
    glcall(glUniformMatrix4fv(GetUniformLocation(program, un_name), 1, GL_FALSE, &matrix[0][0]));
}


void SetUniform3f(unsigned int program, const char* un_name, glm::vec3 vec) {
    glcall(glUniform3f(GetUniformLocation(program, un_name), vec.r, vec.g, vec.b));
}

void SetUniform4f(unsigned int program, const char* un_name, glm::vec4 vec) {
    glcall(glUniform4f(GetUniformLocation(program, un_name), vec.r, vec.g, vec.b, vec.a));
}

*/