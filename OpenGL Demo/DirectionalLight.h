#pragma once

#include "LightSource.h"

class DirectionalLight : public LightSource
{
public:
    DirectionalLight(glm::vec3 direction, glm::vec3 color,
                     float ambientIntensity, float diffuseIntensity);

    void Use(const std::string& uniform, Shader& shader);
private:
    glm::vec3 m_direction;
};

