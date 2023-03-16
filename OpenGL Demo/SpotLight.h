#pragma once

#include "PointLight.h"

class SpotLight : public PointLight
{
public:
    SpotLight(glm::vec3 direction, float edge, glm::vec3 position,
              glm::vec3 attenuation, glm::vec3 color,
              float ambientIntensity, float diffuseIntensity,
              glm::ivec2 shadowDimensions, float nearPlane, float farPlane);

    void Use(const std::string& uniform, Shader& shader);

    void SetPosition(glm::vec3 position) { m_position = position; }
    void SetDirection(glm::vec3 direction) { m_direction = direction; }
private:
    glm::vec3 m_direction;
    float m_edge;
};

