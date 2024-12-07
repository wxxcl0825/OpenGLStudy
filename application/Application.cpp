#include <glad/glad.h>
#include <iostream>
#include "Application.h"

Application* Application::mInstance = nullptr;  // 类静态成员需在类外部定义初始化，且仅能初始化一次

Application::Application() {

}

Application::~Application() {

}

Application* Application::getInstance() {
    if (mInstance == nullptr)
        mInstance = new Application();
    return mInstance;
}

bool Application::init(const int& width, const int& height) {
    mWidth = width;
    mHeight = height;
    
    // 1. 初始化GLFW基本环境
    glfwInit();
    // 设置OpenGL版本4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // 使用核心模式(非立即渲染模式)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 2. 创建窗体对象
    mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGLStudy", NULL, NULL);
    if (mWindow == NULL)
        return false;

    glfwMakeContextCurrent(mWindow);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    return true;
}

bool Application::update() {
    if (glfwWindowShouldClose(mWindow))
        return false;

    // 接收并分发窗体信息
    glfwPollEvents();

     // 切换双缓存
    glfwSwapBuffers(mWindow);
    return true;
}

void Application::destroy() {
    // 4. 退出程序前做相关清理
    glfwTerminate();
}