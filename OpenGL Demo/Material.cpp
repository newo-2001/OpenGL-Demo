#include "Material.h"

Material::Material(float specularIntensity, float shininess) :
    m_specularIntensity(specularIntensity),
    m_shininess(shininess)
{ }

void Material::Use(Shader& shader)
{
    shader.SetUniform(Uniform<float> { "material.specularIntensity", m_specularIntensity });
    shader.SetUniform(Uniform<float> { "material.shininess", m_shininess });
}
