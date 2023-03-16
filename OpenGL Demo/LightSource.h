#pragma once

#include <GL/glew.h>
#include <glm/ext/vector_float3.hpp>
#include <glm/mat4x4.hpp>

#include "Shader.h"
#include "ShadowMap.h"

class LightSource
{
public:
    LightSource(glm::vec3 color, float ambientIntensity, float diffuseIntensity,
                glm::ivec2 shadowDimensions, std::unique_ptr<ShadowMap>&& shadowMap,
                glm::mat4 lightProjection);

    const std::unique_ptr<ShadowMap>& GetShadowMap() const { return m_shadowMap; }

    void Toggle() { m_isOn = !m_isOn; }

    virtual void Use(const std::string& uniform, Shader& shader);
protected:
    glm::vec3 m_color;
    float m_ambientIntensity;
    float m_diffuseIntensity;
    bool m_isOn = true;

    glm::mat4 m_lightProjection;
    std::unique_ptr<ShadowMap> m_shadowMap;
};
