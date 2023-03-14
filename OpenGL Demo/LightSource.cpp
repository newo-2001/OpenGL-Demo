#include "LightSource.h"

LightSource::LightSource(glm::vec3 color, float ambientIntensity, float diffuseIntensity) :
    m_ambientIntensity(ambientIntensity),
    m_diffuseIntensity(diffuseIntensity),
    m_color(color)
{ }

void LightSource::Use(const std::string& uniform, Shader& shader)
{
    shader.SetUniform(Uniform<float> { uniform + ".ambientIntensity", m_ambientIntensity });
    shader.SetUniform(Uniform<float> { uniform + ".diffuseIntensity", m_diffuseIntensity });
    shader.SetUniform(Uniform<glm::vec3> { uniform + ".color", m_color });
}
