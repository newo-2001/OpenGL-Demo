#pragma once

#include <GL/glew.h>
#include <glm/ext/vector_float3.hpp>

#include "Shader.h"

class LightSource
{
public:
    LightSource(glm::vec3 color, float ambientIntensity, float diffuseIntensity);

    virtual void Use(const std::string& uniform, Shader& shader);
protected:
    glm::vec3 m_color;
    float m_ambientIntensity;
    float m_diffuseIntensity;
};
