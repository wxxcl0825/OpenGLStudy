#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void frameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
    std::cout << "size: " << width << ", " << height << std::endl;
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
    // 1. 初始化GLFW基本环境
    glfwInit();
    // 设置OpenGL版本4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // 使用核心模式(非立即渲染模式)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 2. 创建窗体对象
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGLStudy", NULL, NULL);
    glfwMakeContextCurrent(window);

    // 设置监听
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallBack);
    glfwSetKeyCallback(window, keyCallBack);

    // 使用glad加载所有当前版本的openGL函数
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 3. 执行窗体循环
    while (!glfwWindowShouldClose(window)) {
        // 接收并分发窗体信息
        glfwPollEvents();
    }

    // 4. 退出程序前做相关清理
    glfwTerminate();

    return 0;
}