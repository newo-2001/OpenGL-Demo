#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Window
{
public:
    Window(int width, int height, std::string title);
    ~Window();
    
    bool HasFocus() { return m_hasFocus; }
    void GainFocus();
    void LoseFocus();

    void SetTitle(std::string title);
    const std::string& GetTitle() const;

    void Resize(int width, int height);
    glm::ivec2 GetDimensions() const;

    void SetBackgroundColor(glm::vec4 color);

    bool IsClosed() const;
    void Update();
private:
    bool m_hasFocus = true;
    std::string m_title;
    GLFWwindow* m_window;
};
