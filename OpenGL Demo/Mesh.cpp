#include "Mesh.h"

Mesh::Mesh(const GLfloat* const vertices, size_t vertexCount, const unsigned int* const indices, GLsizei indexCount)
    : m_indices(indexCount)
{
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount, vertices, GL_STATIC_DRAW);
    
    int stride = sizeof(GLfloat) * 5;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*) (sizeof(GLfloat) * 3));

    for (size_t i = 0; i < 2; i++)
    {
        glEnableVertexAttribArray(i);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::Render() const
{
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    if (m_texture)
    {
        m_texture->Use();
    }
    
    glDrawElements(GL_TRIANGLES, m_indices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_IBO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}