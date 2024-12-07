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

    // 设置监听
    // GLFW的回调函数需要一个静态函数或全局函数的指针; 若为成员函数的指针, 由于成员函数会访问非静态成员对象, 从而需隐式包含成员指针
    // 不是找不到指针, 而是指针需要提供一个this, 需要具体的成员
    glfwSetFramebufferSizeCallback(mWindow, frameBufferSizeCallBack);
    glfwSetKeyCallback(mWindow, keyCallBack);

    glfwSetWindowUserPointer(mWindow, this);    // 将this存到Window里

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

void Application::frameBufferSizeCallBack(GLFWwindow* window, int width, int height) {
    Application* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mResizeCallback != nullptr)
        self->mResizeCallback(width, height);

    // if (Application::getInstance()->mResizeCallback != nullptr)
    //     Application::getInstance()->mResizeCallback(width, height);
}

void Application::keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->mKeyBoardCallBack != nullptr)
        self->mKeyBoardCallBack(key, action, mods);
}