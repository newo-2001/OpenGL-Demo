#include <memory>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Scene.h"
#include "Game.h"
#include <format>
#include "Model.h"

Scene::Scene(const Window& window)
{
    glm::ivec2 dimensions = window.GetDimensions();
    GLfloat aspectRatio = (GLfloat) dimensions.x / dimensions.y;

    m_projectionMatrix = glm::perspective(45.0f, aspectRatio, 0.1f, 100.0f);
}

void Scene::Render() const
{
    if (!m_shader) return;

    std::shared_ptr<Camera> camera = Game::GetInstance().GetCamera().lock();

    m_shader->Bind();
    
    m_directionalLight->Use("directionalLight", *m_shader);
    
    m_shader->SetUniform(Uniform<int> { "pointLightCount", (int) m_pointLightCount });
    for (size_t i = 0; i < m_pointLightCount; i++)
    {
        PointLight& light = *m_pointLights[i];
        light.Use(std::format("pointLights[{}]", i), *m_shader);
    }

    m_shader->SetUniform(Uniform<int> { "spotLightCount", (int) m_spotLightCount });
    for (size_t i = 0; i < m_spotLightCount; i++)
    {
        SpotLight& light = *m_spotLights[i];
        light.Use(std::format("spotLights[{}]", i), *m_shader);
    }

    m_shader->SetUniform(Uniform<glm::mat4> { "view", camera->GetViewMatrix() });
    m_shader->SetUniform(Uniform<glm::mat4> { "projection", m_projectionMatrix });
    m_shader->SetUniform(Uniform<glm::vec3> { "cameraPos", camera->GetPosition() });
    
    for (const std::shared_ptr<GameObject>& object : m_objects)
    {
        object->Render(*m_shader);
    }

    m_shader->Unbind();
}

void Scene::UseShader(std::shared_ptr<Shader>& shader)
{
    m_shader = shader;
}

void Scene::AddObject(std::shared_ptr<GameObject> object)
{
    m_objects.push_back(object);
}

void Scene::AddPointLight(std::unique_ptr<PointLight>& light)
{
    if (m_pointLightCount < m_pointLights.size())
    {
        m_pointLights[m_pointLightCount++] = std::move(light);
    }
}

void Scene::AddSpotLight(std::unique_ptr<SpotLight>& light)
{
    if (m_spotLightCount < m_spotLights.size())
    {
        m_spotLights[m_spotLightCount++] = std::move(light);
    }
}

void Scene::AddDirectionalLight(std::unique_ptr<DirectionalLight>& light)
{
    m_directionalLight = std::move(light);
}