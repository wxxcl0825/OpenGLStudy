#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "wrapper/checkError.h"
#include "application/Application.h"

GLuint program, vao;

void OnResize(int width, int height) {
    GL_CALL(glViewport(0, 0, width, height));
}

void onKey(int key, int action, int mods) {
    // if (key == GLFW_KEY_W) {

    // }

    // if (action == GLFW_PRESS) {

    // }

    // if (action == GLFW_RELEASE) {

    // }

    // if (mods == GLFW_MOD_CONTROL) {

    // }

    // if (mods == GLFW_MOD_SHIFT) {

    // }
    std::cout << "Key Pressed: " << key << std::endl;
    std::cout << "action: " << action << std::endl;
    std::cout << "mods: " << mods << std::endl;
}

void prepareSingleBuffer() {
    // 为数据各自生成一个vbo, 为分开的vbo各自填充数据
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    float colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    GLuint posVbo = 0, colorVbo = 0;
    GL_CALL(glGenBuffers(1, &posVbo));
    GL_CALL(glGenBuffers(1, &colorVbo));
    
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW));

    // 生成vao, 并分别加入vao
    GLuint vao = 0;
    GL_CALL(glGenVertexArrays(1, &vao));
    GL_CALL(glBindVertexArray(vao));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, posVbo));  // 绑定vbo, 下面属性描述才能与该vbo相关
    GL_CALL(glEnableVertexAttribArray(0));  // vao的0号位
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));  // 压入属性描述

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, colorVbo));
    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0));

    GL_CALL(glBindVertexArray(0));  // vao解绑
}

void prepareInterleavedBuffer() {
    // 将所有数据放在一个vbo里
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    GLuint vbo = 0;
    GL_CALL(glGenBuffers(1, &vbo));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GL_CALL(glGenVertexArrays(1, &vao));
    GL_CALL(glBindVertexArray(vao));

    // glBindBuffer(GL_ARRAY_BUFFER, vbo);  //  已绑定

    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));

    GL_CALL(glEnableVertexAttribArray(1));
    GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));

    GL_CALL(glBindVertexArray(0));
}

void prepareShader() {
    const char* vertexShaderSource = 
        "#version 410 core\n"
        "layout (location = 0) in vec3 aPos;\n" // layout (location = 0) 选择VAO对应条目; in 声明输入
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char* fragmentShaderSource = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    // 创建源代码(相当于新建代码文件)
    GLuint vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);

    // 输入Shader代码(相当于将代码输入文件)
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);   //  char**: char*数组, 仅一个字符串; 字符串自然结尾, 无需指定长度
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);

    // 编译Shader代码
    int success = 0;
    char infoLog[1024];
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success); // 检查编译是否成功
    if (!success) {
        glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
        std::cout << "Error: SHADER COMPILE ERROR - VERTEX" << "\n" << infoLog << std::endl;
    }

    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success); // 检查编译是否成功
    if (!success) {
        glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
        std::cout << "Error: SHADER COMPILE ERROR - FRAGMENT" << "\n" << infoLog << std::endl;
    }

    // 创建一个Program(空壳)
    program = glCreateProgram();

    // 将编译结果放到壳子里
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    // 链接
    glLinkProgram(program);

    // 检查链接错误
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cout << "Error: SHADER LINK ERROR" << "\n" << infoLog << std::endl;
    }

    // 清理两段源代码
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void render() {
    // 清理画布
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

    // 渲染操作
    // 绑定program(选择材质)
    glUseProgram(program);

    // 绑定vao(选择几何信息)
    glBindVertexArray(vao);

    // 发出绘制指令
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main() {
    if (!app->init(800, 600))
        return -1;

    app->setResizeCallback(OnResize);
    app->setKeyBoardCallBack(onKey);

    // 设置openGL视口并清理颜色
    glViewport(0, 0, 800, 600);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // 设置用于Clear时的颜色, 以便在Clear时将整个画布设置为该颜色

    prepareShader();
    prepareInterleavedBuffer();

    // 执行窗体循环
    while (app->update()) {
        render();
    }

    app->destroy();
    return 0;
}