#include <stdexcept>
#include <format>

#include "Texture.h"

Texture::Texture(const std::string& filePath, bool hasAlpha)
{
    unsigned char* data = stbi_load(filePath.c_str(), &m_dimensions.x, &m_dimensions.y, &m_bitDepth, 0);
    if (!data)
    {
        throw std::runtime_error(std::format("Failed to open file: {}", filePath));
    }

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLenum colorSpace = hasAlpha ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, colorSpace, m_dimensions.x, m_dimensions.y, 0, colorSpace, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_texture);
}

void Texture::Use()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}
