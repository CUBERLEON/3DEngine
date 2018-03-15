#include "Window.h"

#include "../core/Debug.h"
#include "../core/Input.h"

Window::Window(int width, int height, const std::string& title, bool fullscreen, bool vsync) : m_title(title)
{
    if (!glfwInit())
        Debug::fatalError("glfw initialization failed");

    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWmonitor* monitor = nullptr;
    if (fullscreen)
        monitor = glfwGetPrimaryMonitor();

    m_glfwWindow = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);

    if (!m_glfwWindow)
    {
        glfwTerminate();
        Debug::fatalError("Creation of Window failed");
    }

    glfwMakeContextCurrent(m_glfwWindow);

    if (vsync)
        glfwSwapInterval(1);

    moveToScreenCenter();
    m_input = new Input(this);
    m_input->setCursor(false);

    GLenum err = glewInit();
    if (err != GLEW_OK)
        Debug::fatalError("%s", glewGetErrorString(err));

    Debug::info("Created window '" + title + "' " + std::to_string(width) + "x" + std::to_string(height));
}

Window::~Window()
{
    Debug::info("Window destructor");
    glfwDestroyWindow(m_glfwWindow);
    glfwTerminate();
}

void Window::refresh()
{
    if (isCreated())
    {
        glfwSwapBuffers(m_glfwWindow);
        glfwPollEvents();
    }
}

void Window::resize(int width, int height)
{
    glfwSetWindowSize(m_glfwWindow, width, height);
}

bool Window::isCloseRequested()
{
    return glfwWindowShouldClose(m_glfwWindow) != 0;
}

bool Window::isCreated()
{
    return m_glfwWindow != nullptr;
}

bool Window::isFocused()
{
    return glfwGetWindowAttrib(m_glfwWindow, GLFW_FOCUSED) != 0;
}

int Window::getWidth()
{
    int width, height;
    glfwGetWindowSize(m_glfwWindow, &width, &height);
    return width;
}

int Window::getHeight()
{
    int width, height;
    glfwGetWindowSize(m_glfwWindow, &width, &height);
    return height;
}

std::string Window::getTitle()
{
    return m_title;
}

glm::vec2 Window::getCenter()
{
    int width, height;
    glfwGetWindowSize(m_glfwWindow, &width, &height);
    return glm::vec2(width / 2, height / 2);
}

float Window::getAspectRatio()
{
    int width, height;
    glfwGetWindowSize(m_glfwWindow, &width, &height); //getting window's sizes
    return width / (float)height; //returning its aspect ratio
}

Input* Window::getInput()
{
    return m_input;
}

GLFWwindow* Window::getGLFWwindow()
{
    return m_glfwWindow;
}

void Window::moveToScreenCenter()
{
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    int width, height;
    glfwGetWindowSize(m_glfwWindow, &width, &height);
    glfwSetWindowPos(m_glfwWindow, (mode->width - width) / 2, (mode->height - height) / 2);
}