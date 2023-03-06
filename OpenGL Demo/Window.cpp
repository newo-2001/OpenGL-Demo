#include "Window.h"
#include <stdexcept>
#include <glm/vec4.hpp>

void initializeGlfw();

void onResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Window::Window(int width, int height, std::string title)
    : m_title(title)
{
    initializeGlfw();

    m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        throw std::runtime_error("GLFW Failed to create window");
    }
    
    glfwMakeContextCurrent(m_window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLEW");
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(m_window, &bufferWidth, &bufferHeight);
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowSizeCallback(m_window, onResize);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

const std::string& Window::getTitle() const
{
    return m_title;
}

void Window::setTitle(std::string title)
{
    glfwSetWindowTitle(m_window, title.c_str());
    m_title = title;
}

void Window::resize(int width, int height)
{
    glfwSetWindowSize(m_window, width, height);
}

glm::ivec2 Window::getDimensions() const
{
    glm::ivec2 dimensions(0);
    glfwGetFramebufferSize(m_window, &dimensions.x, &dimensions.y);

    return dimensions;
}

void Window::setBackgroundColor(glm::vec4 color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

bool Window::isClosed() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::update()
{
    glfwPollEvents();

    glfwSwapBuffers(m_window);
    glClear(GL_COLOR_BUFFER_BIT);
}

void initializeGlfw()
{
    if (!glfwInit())
    {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}