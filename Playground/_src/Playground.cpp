#include "Cream/Core/Entrypoint.h"
#include <iostream>

//Temporary
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class VulkanTriangle
{
public:
    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    void initWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        m_Window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    void initVulkan()
    {

    }

    void mainLoop()
    {
        while (!glfwWindowShouldClose(m_Window))
        {
            glfwPollEvents();
        }

    }

    void cleanup()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

private:
    GLFWwindow* m_Window;
};






class Playground : public Cream::Application
{
public:
	Playground()
	{
        VulkanTriangle app;
        try
        {
            app.run();
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
	}
};

Cream::Ref<Cream::Application> Cream::CreateApplication()
{
	return Cream::CreateRef<Cream::Application>(Playground());
}