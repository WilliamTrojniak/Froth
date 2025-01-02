#pragma once

#include "GLFW/glfw3.h"
#include "Window.h"

namespace Froth {
class GLFWWindow : public Window {
  friend class Window;

private:
  GLFWwindow *m_Window;
  void windowCloseCallback();
  static void windowCloseCallback(GLFWwindow *window);

protected:
  GLFWWindow(int width, int height, const char *title);

public:
  static void pollEvents();
  ~GLFWWindow();
};
} // namespace Froth
