#include <memory>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Scene.h"
#include "Game.h"

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
    m_shader->SetUniform("view", camera->GetViewMatrix());
    m_shader->SetUniform("projection", m_projectionMatrix);
    
    for (const std::shared_ptr<Mesh> mesh : m_meshes)
    {
        mesh->Render();
    }

    m_shader->Unbind();
}

void Scene::UseShader(std::shared_ptr<Shader>& shader)
{
    m_shader = shader;
}

void Scene::AddMesh(std::shared_ptr<Mesh>& mesh)
{
    m_meshes.push_back(mesh);
}