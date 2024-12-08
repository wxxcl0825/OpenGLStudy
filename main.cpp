#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "wrapper/checkError.h"
#include "application/Application.h"

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

void prepareVBO() {
    // VBO: GPU显存数据段的编号
    // 创建一个VBO(显存未分配)
    GLuint vbo = 0;
    GL_CALL(glGenBuffers(1, &vbo));

    // 销毁一个VBO
    GL_CALL(glDeleteBuffers(1, &vbo));

    // 创建n个VBO
    GLuint vboArr[] = {0, 0, 0};
    GL_CALL(glGenBuffers(3, vboArr));

    // 销毁n个VBO
    GL_CALL(glDeleteBuffers(3, vboArr));
    
}

void prepare() {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    GLuint vbo;
    GL_CALL(glGenBuffers(1, &vbo));

    // 绑定VBO到openGL插槽
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));  // VBO插槽: GL_ARRAY_BUFFER

    // 向当前VBO传输数据, 开辟显存
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));  // 静态数据GL_STATIC_DRAW, 动态数据GL_DYNAMIC_DRAW
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
}

int main() {
    if (!app->init(800, 600))
        return -1;

    app->setResizeCallback(OnResize);
    app->setKeyBoardCallBack(onKey);

    // 设置openGL视口并清理颜色
    glViewport(0, 0, 800, 600);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // 设置用于Clear时的颜色, 以便在Clear时将整个画布设置为该颜色

    prepareInterleavedBuffer();

    // 执行窗体循环
    while (app->update()) {
        // 清理画布
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        // 渲染操作

    }

    app->destroy();
    return 0;
}