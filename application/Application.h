#pragma once

#include <glfw/glfw3.h>
#include <cstdint>


#define app Application::getInstance()

// 表示当前应用程序本身
class Application {
public:
  ~Application();
  static Application *getInstance(); // 访问实例

  bool init(const int& width = 800, const int& height = 600);
  bool update();
  void destroy();

  uint32_t getWidth() const { return mWidth; };
  uint32_t getHeight() const { return mHeight; };

private:
  static Application *mInstance; // 全局唯一的静态实例

  uint32_t mWidth{0};
  uint32_t mHeight{0};
  GLFWwindow *mWindow{nullptr};

  Application();                 // 私有构造函数, 防止外界new出实例
};