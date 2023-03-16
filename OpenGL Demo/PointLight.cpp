#include "PointLight.h"

#include <numbers>
#include <glm/ext/matrix_clip_space.hpp>
#include "OmniDirectionalShadowMap.h"
#include <glm/ext/matrix_transform.hpp>

PointLight::PointLight(glm::vec3 position, glm::vec3 attenuation,
                       glm::vec3 color, float ambientIntensity, float diffuseIntensity,
                       glm::ivec2 shadowDimensions, float nearPlane, float farPlane) :
    LightSource(color, ambientIntensity, diffuseIntensity, shadowDimensions,
                std::make_unique<OmniDirectionalShadowMap>(shadowDimensions),
                glm::perspective((float) std::numbers::pi / 2,
                    (float) shadowDimensions.x / shadowDimensions.y,
                    nearPlane, farPlane)),
    
    m_position(position),
    m_attenuation(attenuation),
    m_nearPlane(nearPlane),
    m_farPlane(farPlane)
{ }

void PointLight::Use(const std::string& uniform, Shader& shader)
{
    shader.SetUniform(Uniform<float> { uniform + ".attenuation.quadratic", m_attenuation.x });
    shader.SetUniform(Uniform<float> { uniform + ".attenuation.linear", m_attenuation.y });
    shader.SetUniform(Uniform<float> { uniform + ".attenuation.constant", m_attenuation.z });
    shader.SetUniform(Uniform<float> { uniform + ".farPlane", m_farPlane });
    shader.SetUniform(Uniform<glm::vec3> { uniform + ".position", m_position });

    LightSource::Use(uniform + ".base", shader);
}

std::array<glm::mat4, 6> PointLight::CalculateLightTransforms() const
{
    return std::array<glm::mat4, 6>
    {
        m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3 { 1.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, -1.0f, 0.0f }),
        m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3 { -1.0f, 0.0f, 0.0f }, glm::vec3 { 0.0f, -1.0f, 0.0f }),
        m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3 { 0.0f, 1.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, 1.0f }),
        m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3 { 0.0f, -1.0f, 0.0f }, glm::vec3 { 0.0f, 0.0f, -1.0f }),
        m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3 { 0.0f, 0.0f, 1.0f }, glm::vec3 { 0.0f, -1.0f, 0.0f }),
        m_lightProjection * glm::lookAt(m_position, m_position + glm::vec3 { 0.0f, 0.0f, -1.0f }, glm::vec3 { 0.0f, -1.0f, 0.0f }),
    };
}
