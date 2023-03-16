#include "SpotLight.h"
#include <glm/trigonometric.hpp>
#include <glm/geometric.hpp>


SpotLight::SpotLight(glm::vec3 direction, float edge, glm::vec3 position,
                     glm::vec3 attenuation, glm::vec3 color,
                     float ambientIntensity, float diffuseIntensity,
                     glm::ivec2 shadowDimensions, float nearPlane, float farPlane) :
    PointLight(position, attenuation, color, ambientIntensity, diffuseIntensity, shadowDimensions, nearPlane, farPlane),
    m_direction(glm::normalize(direction)),
    m_edge(cosf(glm::radians(edge)))
{ }

void SpotLight::Use(const std::string& uniform, Shader& shader)
{
    shader.SetUniform(Uniform<glm::vec3> { uniform + ".direction", m_direction });
    shader.SetUniform(Uniform<float> { uniform + ".edge", m_edge });

    PointLight::Use(uniform + ".base", shader);
}
