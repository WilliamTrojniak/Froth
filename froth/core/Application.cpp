#define GLFW_INCLUDE_VULKAN
#include "core/Application.h"
#include <GLFW/glfw3.h>

namespace Froth {
Application::Application() {

  if (!glfwInit()) {
  }
  GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (window) {
    glfwMakeContextCurrent(window);
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
    }
    glfwDestroyWindow(window);
  }

  glfwTerminate();
}
Application::~Application() {}

void Application::Run() {}

} // namespace Froth
