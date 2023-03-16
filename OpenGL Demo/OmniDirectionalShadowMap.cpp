#include "OmniDirectionalShadowMap.h"
#include <format>

OmniDirectionalShadowMap::OmniDirectionalShadowMap(glm::ivec2 shadowDimensions)
{
    m_shadowDimensions = shadowDimensions;

    glGenFramebuffers(1, &m_FBO);

    glGenTextures(1, &m_shadowMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
    
    for (size_t i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                     shadowDimensions.x, shadowDimensions.y, 0, GL_DEPTH_COMPONENT,
                     GL_FLOAT, nullptr);
    }

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowMap, 0);

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
}

void OmniDirectionalShadowMap::Write()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
}

void OmniDirectionalShadowMap::Read(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
}

OmniDirectionalShadowMap::~OmniDirectionalShadowMap() { }
