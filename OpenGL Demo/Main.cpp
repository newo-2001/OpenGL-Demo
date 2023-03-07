#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Window.h"
#include "Shader.h"

Window* window;
Shader* shader;
std::vector<Mesh> meshes;


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

Mesh createTriangle()
{
    constexpr GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
         0.0f, -1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f
    };

    constexpr unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    meshes.push_back(Mesh(vertices, 12, indices, 2));
}

void draw()
{
    shader->Bind();

    for (Mesh& mesh : meshes)
    {
        mesh.Render();
    }

    shader->Unbind();
}

void initialize()
{
    window = new Window(800, 600, "Hello World!");
    window->SetBackgroundColor(glm::vec4 { 0.35f, 0.75f, 0.66f, 1.0f });

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
