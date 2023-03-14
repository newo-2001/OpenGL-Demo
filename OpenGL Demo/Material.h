#pragma once
#include "Shader.h"
class Material
{
public:
    Material(float specularIntensity = 0.0f, float shininess = 0.0f);

    void Use(Shader& shader);
private:
    float m_specularIntensity, m_shininess;
};

