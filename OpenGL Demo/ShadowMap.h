#pragma once

#include <stdio.h>
#include <glm/ext/vector_int2.hpp>
#include <GL/glew.h>

class ShadowMap
{
public:
    ShadowMap(glm::ivec2 shadowDimensions);
    ~ShadowMap();

    void Write();
    void Read(GLenum textureUnit);

    glm::ivec2 GetDimensions() const { return m_shadowDimensions; }
protected:
    GLuint m_FBO = 0, m_shadowMap = 0;
    glm::ivec2 m_shadowDimensions;
};

