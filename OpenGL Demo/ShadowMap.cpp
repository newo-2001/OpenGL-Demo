#include "ShadowMap.h"
#include <format>

#include "GL\glew.h"

ShadowMap::ShadowMap() {}

ShadowMap::ShadowMap(glm::ivec2 shadowDimensions)
    : m_shadowDimensions(shadowDimensions)
{
    glGenFramebuffers(1, &m_FBO);

    glGenTextures(1, &m_shadowMap);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 shadowDimensions.x, shadowDimensions.y,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);
    
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        glDeleteFramebuffers(1, &m_FBO);
        glDeleteTextures(1, &m_shadowMap);

        const char* error = (const char*) glewGetErrorString(status);
        throw std::runtime_error(std::format("Failed to create framebuffer: {}", error));
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::Write()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

void ShadowMap::Read(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_shadowMap);
}

ShadowMap::~ShadowMap()
{
    glDeleteFramebuffers(1, &m_FBO);
    glDeleteTextures(1, &m_shadowMap);
}
