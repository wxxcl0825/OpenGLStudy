#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "shader.h"
#include "../wrapper/checkError.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR: Shader File Error: " << e.what() << std::endl;
    }

    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    // 创建源代码(相当于新建代码文件)
    GLuint vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);

    // 输入Shader代码(相当于将代码输入文件)
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);   //  char**: char*数组, 仅一个字符串; 字符串自然结尾, 无需指定长度
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

    // 编译Shader代码
    glCompileShader(vertex);
    // 检查编译是否成功
    checkShaderErrors(vertex, "COMPILE");

    glCompileShader(fragment);
    checkShaderErrors(fragment, "COMPILE");
    
    // 创建一个Program(空壳)
    mProgram = glCreateProgram();

    // 将编译结果放到壳子里
    glAttachShader(mProgram, vertex);
    glAttachShader(mProgram, fragment);

    // 链接
    glLinkProgram(mProgram);

    // 检查链接错误
    checkShaderErrors(mProgram, "LINK");

    // 清理两段源代码
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

Shader::~Shader() {

}

void Shader::begin() {
   GL_CALL(glUseProgram(mProgram));
}

void Shader::end() {
    GL_CALL(glUseProgram(0));
}

void Shader::checkShaderErrors(GLuint target, std::string type) {
    int success = 0;
    char infoLog[1024];

    if (type == "COMPILE") {
        glGetShaderiv(target, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(target, 1024, NULL, infoLog);
            std::cout << "Error: SHADER COMPILE ERROR" << "\n" << infoLog << std::endl;
        }
    } else if (type == "LINK") {
        glGetProgramiv(target, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(target, 1024, NULL, infoLog);
            std::cout << "Error: SHADER LINK ERROR" << "\n" << infoLog << std::endl;
        }
    } else {
        std::cout << "ERROR: checkShaderErrors with wrong type." << std::endl;
    }
}

void Shader::setFloat(const std::string &name, float value) {
    GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
    GL_CALL(glUniform1f(location, value));
}

void Shader::setVector3(const std::string &name, float x, float y, float z) {
    GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
    GL_CALL(glUniform3f(location, x, y, z));
}

void Shader::setVector3(const std::string &name, float* values) {
    GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
    GL_CALL(glUniform3fv(location, 1, values));
}

void Shader::setInt(const std::string &name, int value) {
    GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
    GL_CALL(glUniform1i(location, value));
}

void Shader::setMatrix4x4(const std::string &name, glm::mat4 value) {
    GLint location = GL_CALL(glGetUniformLocation(mProgram, name.c_str()));
    GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}