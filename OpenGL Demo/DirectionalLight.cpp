#include "DirectionalLight.h"
#include "Shader.h"

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color,
                                   float ambientIntensity, float diffuseIntensity) :
    LightSource(color, ambientIntensity, diffuseIntensity),
    m_direction(direction)
{ }

void DirectionalLight::Use(const std::string& uniform, Shader& shader)
{
    shader.SetUniform(Uniform<glm::vec3> { uniform + ".direction", m_direction });

    LightSource::Use(uniform + ".base", shader);
}
