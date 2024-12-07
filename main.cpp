#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
    
    // 3. 执行窗体循环
    while (!glfwWindowShouldClose(window)) {
        // 接收并分发窗体信息
        glfwPollEvents();
    }

    // 4. 退出程序前做相关清理
    glfwTerminate();

    return 0;
}