#pragma once

#include "LightSource.h"
#include <glm/ext/matrix_float4x4.hpp>

class DirectionalLight : public LightSource
{
public:
    DirectionalLight(glm::vec3 direction, glm::vec3 color,
                     float ambientIntensity, float diffuseIntensity, glm::ivec2 shadowDimensions);

    void Use(const std::string& uniform, Shader& shader);

    glm::mat4 CalculateLightTransform() const;
private:
    glm::vec3 m_direction;
};

