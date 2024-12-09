#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
    void setInt(const std::string &name, int value);
    void setMatrix4x4(const std::string &name, glm::mat4 value);
private:
    GLuint mProgram{0};

    void checkShaderErrors(GLuint target, std::string type);  // 检查编译/链接错误
};