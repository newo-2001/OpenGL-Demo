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
    
    void setTitle(std::string title);
    const std::string& getTitle() const;

    void resize(int width, int height);
    glm::ivec2 getDimensions() const;

    void setBackgroundColor(glm::vec4 color);

    bool isClosed() const;
    void update();
private:
    std::string m_title;
    GLFWwindow* m_window;
};
