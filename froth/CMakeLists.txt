cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(HOME "$ENV{HOME}")

project(Froth VERSION 0.1)
configure_file(Defines.h.in Defines.h)

add_library(${PROJECT_NAME}
  core/events/ApplicationEvent.h
  core/events/Event.h
  core/events/EventDispatcher.h
  core/events/KeyEvent.h
  core/logger/Logger.h
  core/logger/Logger.cpp
  core/Application.h
  core/Application.cpp
  core/Entrypoint.h
  core/Entrypoint.cpp
  core/Layer.h
  core/LayerStack.h
  core/LayerStack.cpp
  platform/keys/GLFWCodes.h
  platform/keys/GLFWCodes.cpp
  platform/keys/Keycodes.h
  platform/keys/Mousecodes.h
  platform/window/GLFWWindow.h
  platform/window/GLFWWindow.cpp
  platform/window/Window.h
  platform/window/Window.cpp
  dependencies/vendor/stb/stb/stb_image.h
  dependencies/vendor/tiny_obj_loader/tiny_obj_loader.h
)

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${PROJECT_BINARY_DIR}
  ./ 
  dependencies/vendor/glm
  dependencies/vendor/glfw/include
  dependencies/vendor/stb
  dependencies/vendor/tiny_obj_loader
  ${HOME}/VulkanSDK/1.3.296.0/macOS/include
)

add_subdirectory(dependencies/vendor/glm)
add_subdirectory(dependencies/vendor/glfw)

target_link_libraries(${PROJECT_NAME} PUBLIC glm)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

add_library(vulkan1 SHARED IMPORTED GLOBAL)
add_library(vulkan1xx SHARED IMPORTED GLOBAL)
set_target_properties(vulkan1 PROPERTIES IMPORTED_LOCATION ${HOME}/VulkanSDK/1.3.296.0/macOS/lib/libvulkan.1.dylib)
set_target_properties(vulkan1xx PROPERTIES IMPORTED_LOCATION ${HOME}/VulkanSDK/1.3.296.0/macOS/lib/libvulkan.1.3.296.dylib)
target_link_libraries(${PROJECT_NAME} PUBLIC vulkan1 vulkan1xx)
