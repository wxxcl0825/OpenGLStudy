#pragma once

#include <glfw/glfw3.h>
#include <cstdint>


#define app Application::getInstance()

using ResizeCallback = void(*)(int width, int height);
using KeyBoardCallback = void(*)(int key, int action, int mods);
using MouseCallback = void(*)(int button, int action, int mods);
using CursorCallback = void(*)(double xpos, double ypos);
using ScrollCallback = void(*)(double offset);

// 表示当前应用程序本身

/*
    回调函数:
    1. 声明回调函数指针类型
    2. 声明私有函数指针, 用于保存外部传入的回调函数
    3. 设置回调函数成员的setter方法
    4. 声明静态回调函数用于glfw设置, 使用UserPointer调用回调函数成员
    5. 在init绑定回调函数
    6. 在main.cpp定义回调函数, 并调用setter方法设置回调成员
*/

class Application {
public:
  ~Application();
  static Application *getInstance(); // 访问实例

  bool init(const int& width = 800, const int& height = 600);
  bool update();
  void destroy();

  uint32_t getWidth() const { return mWidth; };
  uint32_t getHeight() const { return mHeight; };

  void getCursorPosition(double *x, double *y);
  GLFWwindow* getWindow() { return mWindow; }

  void setResizeCallback(ResizeCallback callback) { mResizeCallback = callback; };
  void setKeyBoardCallback(KeyBoardCallback callback) { mKeyBoardCallback = callback; }
  void setMouseCallback(MouseCallback callback) { mMouseCallback = callback; }
  void setCursorCallback(CursorCallback callback) { mCursorCallback = callback; }
  void setScrollCallback(ScrollCallback callback) { mScrollCallback = callback; }

private:
  static Application *mInstance; // 全局唯一的静态实例

  uint32_t mWidth{0};
  uint32_t mHeight{0};
  GLFWwindow *mWindow{nullptr};

  ResizeCallback mResizeCallback{nullptr};
  KeyBoardCallback mKeyBoardCallback{nullptr};
  MouseCallback mMouseCallback{nullptr};
  CursorCallback mCursorCallback{nullptr};
  ScrollCallback mScrollCallback{nullptr};

  Application();                 // 私有构造函数, 防止外界new出实例

  static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
  static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
  static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};