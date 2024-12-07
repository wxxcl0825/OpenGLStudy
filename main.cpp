#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "wrapper/checkError.h"
#include "application/Application.h"

void frameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
    std::cout << "size: " << width << ", " << height << std::endl;
    glViewport(0, 0, width, height);
}

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // scancode: 物理按键位置
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

int main() {
    if (!app->init(800, 600))
        return -1;

    // 设置监听
    // glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
    // glfwSetKeyCallback(window, keyCallBack);

    // 设置openGL视口并清理颜色
    glViewport(0, 0, 800, 600);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // 设置用于Clear时的颜色, 以便在Clear时将整个画布设置为该颜色

    // 执行窗体循环
    while (app->update()) {
        // 清理画布
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        // 渲染操作

    }

    app->destroy();
    return 0;
}