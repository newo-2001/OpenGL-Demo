#pragma once

#include <string>

#include <GL\glew.h>
#include <stb/stb_image.h>
#include <glm/ext/vector_int2.hpp>

class Texture
{
public:
    Texture(const std::string& filePath, bool hasAlpha = false);
    ~Texture();

    glm::ivec2 GetDimensions() { return m_dimensions; }

    void Use();
private:
    int m_bitDepth;
    glm::ivec2 m_dimensions;
    GLuint m_texture;
};

