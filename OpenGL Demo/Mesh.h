#pragma once

#include <memory>

#include <GL/glew.h>

#include "Texture.h"

class Mesh
{
public:
    Mesh(const GLfloat* const vertices, size_t vertexCount, const unsigned int* const indices, GLsizei indexCount);
    ~Mesh();

    void Render() const;
private:
    GLuint m_VAO, m_VBO, m_IBO;
    GLsizei m_indices;
};
