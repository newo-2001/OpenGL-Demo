#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Window.h"
#include "Shader.h"

Window* window;
Shader* shader;
GLuint VAO, VBO;

void printException(const std::exception& err, int level = 0)
{
    std::cerr << std::string(level * 4, ' ') << err.what() << std::endl;
    
    try
    {
        std::rethrow_if_nested(err);
    }
    catch (std::exception& nested)
    {
        printException(nested, level + 1);
    } catch (...) { }
}

void createTriangle()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glEnableVertexAttribArray(0);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
}

void draw()
{
    shader->bind();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    shader->unbind();
}

void initialize()
{
    window = new Window(800, 600, "Hello World!");
    window->setBackgroundColor(glm::vec4 { 0.35f, 0.75f, 0.66f, 1.0f });

    shader = new Shader("vertex.glsl", "fragment.glsl");

    createTriangle();
}

void loop()
{
    while (!window->isClosed())
    {
        draw();

        window->update();
    }
}

int main()
{
    try
    {
        initialize();
        loop();
    }
    catch (std::exception& err)
    {
        delete window;
        delete shader;

        printException(err);
        return 1;
    }

    return 0;
}
