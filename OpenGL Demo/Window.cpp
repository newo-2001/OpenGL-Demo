#include <format>
#include <stdexcept>

#include "Window.h"
#include "Input.h"

#include <glm/vec4.hpp>

void InitializeGlfw();
void OnKeyEvent(GLFWwindow* window, int key, int code, int action, int mode);
void OnMouseMoveEvent(GLFWwindow* window, double x, double y);
void OnMouseButtonEvent(GLFWwindow* window, int button, int action, int mods);

void OnResize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

Window::Window(int width, int height, std::string title)
    : m_title(title)
{
    InitializeGlfw();

    m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!m_window)
    {
        glfwTerminate();
        throw std::runtime_error("GLFW Failed to create window");
    }
    
    glfwMakeContextCurrent(m_window);

    glewExperimental = GL_TRUE;
    
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK)
    {
        const char* error = (const char*) glewGetErrorString(glewStatus);
        
        glfwDestroyWindow(m_window);
        glfwTerminate();

        throw std::runtime_error(std::format("Failed to initialize GLEW, {}", error));
    }

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(m_window, &bufferWidth, &bufferHeight);

    glfwSetWindowUserPointer(m_window, this);

    glfwSetWindowSizeCallback(m_window, OnResize);
    glfwSetKeyCallback(m_window, OnKeyEvent);
    glfwSetCursorPosCallback(m_window, OnMouseMoveEvent);
    glfwSetMouseButtonCallback(m_window, OnMouseButtonEvent);

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, bufferWidth, bufferHeight);

    Input::InitializeInputHandler();

    GainFocus();
    glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

const std::string& Window::GetTitle() const
{
    return m_title;
}

void Window::SetTitle(std::string title)
{
    glfwSetWindowTitle(m_window, title.c_str());
    m_title = title;
}

void Window::Resize(int width, int height)
{
    glfwSetWindowSize(m_window, width, height);
}

glm::ivec2 Window::GetDimensions() const
{
    glm::ivec2 dimensions(0);
    glfwGetFramebufferSize(m_window, &dimensions.x, &dimensions.y);

    return dimensions;
}

glm::ivec2 Window::GetViewPort() const
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);
    
    return glm::ivec2 { viewport[0], viewport[1] };
}

void Window::SetViewPort(int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::SetBackgroundColor(glm::vec4 color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

bool Window::IsClosed() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::Update()
{
    glfwPollEvents();

    glfwSwapBuffers(m_window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::GainFocus()
{
    m_hasFocus = true;
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::LoseFocus()
{
    m_hasFocus = false;
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glm::vec2 center = GetDimensions() / 2;
    glfwSetCursorPos(m_window, center.x, center.y);

    Input::ReleaseAll();
}

void InitializeGlfw()
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

void OnKeyEvent(GLFWwindow* windowPtr, int key, int code, int action, int mode)
{
    Window* window = (Window*) glfwGetWindowUserPointer(windowPtr);

    if (!window->HasFocus()) return;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(windowPtr, GLFW_TRUE);
    }

    Input::OnKeyEvent.Post(Input::KeyEvent{ key, action });
}

void OnMouseButtonEvent(GLFWwindow* windowPtr, int button, int action, int mods)
{
    Window* window = (Window*) glfwGetWindowUserPointer(windowPtr);
    
    if (!window->HasFocus() && button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
    {
        window->GainFocus();
    }
    else if (window->HasFocus() && button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
    {
        window->LoseFocus();
        return;
    }

    if (!window->HasFocus()) return;

    Input::OnMouseButtonEvent.Post(Input::MouseButtonEvent { button, action });
}

void OnMouseMoveEvent(GLFWwindow* windowPtr, double x, double y)
{
    Window* window = (Window*) glfwGetWindowUserPointer(windowPtr);

    static float lastX = (float) x, lastY = (float) y;
    
    float dx = (float) x - lastX;
    float dy = (float) y - lastY;

    lastX = (float) x;
    lastY = (float) y;
    
    if (!window->HasFocus()) return;

    Input::OnMouseMoveEvent.Post(Input::MouseMoveEvent { dx, dy });
}