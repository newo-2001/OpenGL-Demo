#pragma once

#include "LightSource.h"

class PointLight : public LightSource
{
public:
    PointLight(glm::vec3 position, glm::vec3 attenuation,
               glm::vec3 color, float ambientIntensity, float diffuseIntensity);

    void Use(const std::string& uniform, Shader& shader);
protected:
    glm::vec3 m_position;
    glm::vec3 m_attenuation;
};

