#pragma once

#include "LightSource.h"
#include <array>

class PointLight : public LightSource
{
public:
    PointLight(glm::vec3 position, glm::vec3 attenuation,
               glm::vec3 color, float ambientIntensity, float diffuseIntensity,
               glm::ivec2 shadowDimensions,
               float nearPlane, float farPlane);

    void Use(const std::string& uniform, Shader& shader);

    float GetFarPlane() const { return m_farPlane; }
    glm::vec3 GetPosition() const { return m_position; }

    std::array<glm::mat4, 6> CalculateLightTransforms() const;
protected:
    glm::vec3 m_position;
    glm::vec3 m_attenuation;

    float m_nearPlane, m_farPlane;
};

