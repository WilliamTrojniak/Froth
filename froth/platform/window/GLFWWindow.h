#pragma once

#include "GLFW/glfw3.h"
#include "Window.h"

namespace Froth {
class GLFWWindow : public Window {
  friend class Window;

private:
  GLFWwindow *m_Window;
  void windowCloseCallback();
  void windowSizeCallback(int width, int height);
  void keyCallback(int key, int scancode, int action, int mods);
  void mouseButtonCallback(int button, int action, int mods);
  void mouseMoveCallback(double x, double y);
  void mouseScrollCallback(double xOffset, double yOffset);
  static void windowCloseCallback(GLFWwindow *window);
  static void windowSizeCallback(GLFWwindow *window, int width, int height);
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);
  static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                  int mods);
  static void mouseMoveCallback(GLFWwindow *window, double x, double y);
  static void mouseScrollCallback(GLFWwindow *window, double xOffset,
                                  double yOffset);

protected:
  GLFWWindow(int width, int height, const char *title);

public:
  static void pollEvents();
  ~GLFWWindow();
};
} // namespace Froth
