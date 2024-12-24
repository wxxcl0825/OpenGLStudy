#pragma once

#include "../glframework/core.h"
#include <string>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void begin(); // 开始使用当前Shader
    void end(); // 结束使用当前Shader

    void setFloat(const std::string &name, float value);
    void setVector3(const std::string &name, float x, float y, float z);
    void setVector3(const std::string &name, float* values);
    void setVector3(const std::string &name, const glm::vec3 value);
    void setInt(const std::string &name, int value);
    void setMatrix4x4(const std::string &name, glm::mat4 value);
    void setMatrix3x3(const std::string &name, glm::mat3 value);
private:
    GLuint mProgram{0};

    void checkShaderErrors(GLuint target, std::string type);  // 检查编译/链接错误
};