cmake_minimum_required(VERSION 3.12)

set(HOME "$ENV{HOME}")

project(Froth)
add_library(Froth 
  core/events/ApplicationEvent.h
  core/events/Event.h
  core/events/EventDispatcher.h
  core/events/KeyEvent.h
  core/Application.h
  core/Application.cpp
  core/Entrypoint.h
  core/Entrypoint.cpp
  platform/keys/GLFWCodes.h
  platform/keys/GLFWCodes.cpp
  platform/keys/Keycodes.h
  platform/keys/Mousecodes.h
  platform/window/GLFWWindow.h
  platform/window/GLFWWindow.cpp
  platform/window/Window.h
  platform/window/Window.cpp
)

target_include_directories(Froth PRIVATE 
  ./ 
  dependencies/vendor/glfw/include
  ${HOME}/VulkanSDK/1.3.296.0/macOS/include
)

add_subdirectory(dependencies/vendor/glfw)

target_link_libraries(Froth PUBLIC glfw)

add_library(vulkan1 SHARED IMPORTED GLOBAL)
add_library(vulkan1xx SHARED IMPORTED GLOBAL)
set_target_properties(vulkan1 PROPERTIES IMPORTED_LOCATION ${HOME}/VulkanSDK/1.3.296.0/macOS/lib/libvulkan.1.dylib)
set_target_properties(vulkan1xx PROPERTIES IMPORTED_LOCATION ${HOME}/VulkanSDK/1.3.296.0/macOS/lib/libvulkan.1.3.296.dylib)
target_link_libraries(Froth PUBLIC vulkan1 vulkan1xx)
