#include "DirectionalLight.h"
#include "Shader.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color,
                                   float ambientIntensity, float diffuseIntensity,
                                   glm::ivec2 shadowDimensions) :
    LightSource(color, ambientIntensity, diffuseIntensity, shadowDimensions),
    m_direction(direction)
{
    m_lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

void DirectionalLight::Use(const std::string& uniform, Shader& shader)
{
    shader.SetUniform(Uniform<glm::vec3> { uniform + ".direction", m_direction });

    LightSource::Use(uniform + ".base", shader);
}

glm::mat4 DirectionalLight::CalculateLightTransform() const
{
    return m_lightProjection * glm::lookAt(-m_direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
