#include "PointLight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 attenuation,
                       glm::vec3 color, float ambientIntensity, float diffuseIntensity) :
    LightSource(color, ambientIntensity, diffuseIntensity),
    
    m_position(position),
    m_attenuation(attenuation)
{ }

void PointLight::Use(const std::string& uniform, Shader& shader)
{
    shader.SetUniform(Uniform<float> { uniform + ".attenuation.quadratic", m_attenuation.x });
    shader.SetUniform(Uniform<float> { uniform + ".attenuation.linear", m_attenuation.y });
    shader.SetUniform(Uniform<float> { uniform + ".attenuation.constant", m_attenuation.z });
    shader.SetUniform(Uniform<glm::vec3> { uniform + ".position", m_position });

    LightSource::Use(uniform + ".base", shader);
}
