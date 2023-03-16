#include "LightSource.h"

LightSource::LightSource(glm::vec3 color, float ambientIntensity, float diffuseIntensity,
                         glm::ivec2 shadowDimensions, std::unique_ptr<ShadowMap>&& shadowMap,
                         glm::mat4 lightProjection) :
    m_ambientIntensity(ambientIntensity),
    m_diffuseIntensity(diffuseIntensity),
    m_color(color),
    m_shadowMap(std::move(shadowMap)),
    m_lightProjection(lightProjection)
{ }

void LightSource::Use(const std::string& uniform, Shader& shader)
{
    float ambient = m_isOn ? m_ambientIntensity : 0.0f;
    float diffuse = m_isOn ? m_diffuseIntensity : 0.0f;

    shader.SetUniform(Uniform<float> { uniform + ".ambientIntensity", ambient });
    shader.SetUniform(Uniform<float> { uniform + ".diffuseIntensity", diffuse });
    shader.SetUniform(Uniform<glm::vec3> { uniform + ".color", m_color });
}
