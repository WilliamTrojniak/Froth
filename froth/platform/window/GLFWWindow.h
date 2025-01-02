#pragma once

#include "GLFW/glfw3.h"
#include "Window.h"

namespace Froth {
class GLFWWindow : public Window {
  friend class Window;

private:
  GLFWwindow *m_Window;
  void windowCloseCallback();
  void keyCallback(int key, int scancode, int action, int mods);
  static void windowCloseCallback(GLFWwindow *window);
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);

protected:
  GLFWWindow(int width, int height, const char *title);

public:
  static void pollEvents();
  ~GLFWWindow();
};
} // namespace Froth
