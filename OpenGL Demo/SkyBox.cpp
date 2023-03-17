#include "SkyBox.h"

#include <stdexcept>
#include <format>
#include <glm/vec2.hpp>

constexpr unsigned int indices[] {
    0, 1, 2,
    2, 1, 3,
    2, 3, 5,
    5, 3, 7,
    5, 7, 4,
    4, 7, 6,
    4, 6, 0,
    0, 6, 1,
    4, 0, 5,
    5, 0, 2,
    1, 6, 3,
    3, 6, 7
};

constexpr GLfloat vertices[] {
    -1.0f,  1.0f, -1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
     1.0f,  1.0f, -1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, -1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,

    -1.0f,  1.0f,  1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f
};

SkyBox::SkyBox(std::array<std::string, 6> facePaths) :
    m_shader(std::move(Shader::FromFiles("Resources/Shaders/skybox.vertex.glsl", "Resources/shaders/skybox.fragment.glsl"))),
    m_mesh(std::move(std::make_unique<Mesh>(vertices, sizeof(vertices) / sizeof(GLfloat), indices, sizeof(indices) / sizeof(unsigned int))))
{
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
    
    glm::ivec2 dimensions;
    int bitDepth;
    
    for (size_t i = 0; i < facePaths.size(); i++)
    {
        const std::string& path = facePaths[i];
        unsigned char* textureData = stbi_load(path.c_str(), &dimensions.x, &dimensions.y, &bitDepth, 0);
        if (!textureData)
        {
            glDeleteTextures(1, &m_texture);
            throw std::runtime_error(std::format("Failed to open file: {}", path));
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                     dimensions.x, dimensions.y, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, textureData);

        stbi_image_free(textureData);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

SkyBox::~SkyBox()
{
    glDeleteTextures(1, &m_texture);
}

void SkyBox::Render(const glm::mat4& view, const glm::mat4& projection) const
{
    glDepthMask(GL_FALSE);
    m_shader->Bind();

    m_shader->SetUniform(Uniform<glm::mat4> { "projection", projection });
    m_shader->SetUniform(Uniform<glm::mat4> { "view", glm::mat4(glm::mat3(view)) });

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);

    m_shader->Validate();
    m_mesh->Render();

    m_shader->Unbind();
    glDepthMask(GL_TRUE);
}
