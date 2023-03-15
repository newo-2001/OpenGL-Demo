#pragma once

#include <GL/glew.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/mat4x4.hpp>

#include "Shader.h"
#include "ShadowMap.h"

class LightSource
{
public:
    LightSource(glm::vec3 color, float ambientIntensity, float diffuseIntensity, glm::ivec2 shadowDimensions);

    const std::unique_ptr<ShadowMap>& GetShadowMap() const { return m_shadowMap; }

    virtual void Use(const std::string& uniform, Shader& shader);
protected:
    glm::vec3 m_color;
    float m_ambientIntensity;
    float m_diffuseIntensity;

    glm::mat4 m_lightProjection;
    std::unique_ptr<ShadowMap> m_shadowMap;
};
