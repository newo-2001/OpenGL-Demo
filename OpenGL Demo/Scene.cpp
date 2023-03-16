#include <memory>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Scene.h"
#include "Game.h"
#include <format>
#include <numbers>
#include "Model.h"

Scene::Scene(const Window& window) :
    m_directionalShadowShader(std::move(Shader::FromFile("Resources/Shaders/directional_shadow_map.vertex.glsl"))),
    m_omniDirectionalShadowShader(std::move(Shader::FromFiles(
        "Resources/Shaders/omni_directional_shadow_map.vertex.glsl",
        "Resources/Shaders/omni_directional_shadow_map.fragment.glsl",
        "Resources/Shaders/omni_directional_shadow_map.geometry.glsl"
    )))
{
    glm::ivec2 dimensions = window.GetDimensions();
    GLfloat aspectRatio = (GLfloat) dimensions.x / dimensions.y;

    m_projectionMatrix = glm::perspective((float) std::numbers::pi / 3, aspectRatio, 0.1f, 100.0f);
}

void Scene::Render() const
{
    if (!m_shader) return;
    
    Window& window = Game::GetInstance().GetWindow();
    glm::ivec2 windowSize = window.GetDimensions();

    DirectionalShadowMapPass(*m_directionalLight);

    for (size_t i = 0; i < m_pointLightCount; i++)
    {
        OmniDirectionalShadowMapPass(m_pointLights[i]);
    }

    for (size_t i = 0; i < m_spotLightCount; i++)
    {
        OmniDirectionalShadowMapPass(m_spotLights[i]);
    }

    window.SetViewPort(windowSize.x, windowSize.y);

    RenderPass();
}

void Scene::DirectionalShadowMapPass(const DirectionalLight& light) const
{
    m_directionalShadowShader->Bind();
        
    ShadowMap& shadowMap = *light.GetShadowMap();

    Window& window = Game::GetInstance().GetWindow();
    glm::ivec2 dimensions = shadowMap.GetDimensions();
    window.SetViewPort(dimensions.x, dimensions.y);

    shadowMap.Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::mat4 transform = light.CalculateLightTransform();
    m_directionalShadowShader->SetUniform(Uniform<glm::mat4> { "directionalLightTransform", transform });

    RenderScene(*m_directionalShadowShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_directionalShadowShader->Unbind();
}

void Scene::OmniDirectionalShadowMapPass(const std::shared_ptr<PointLight>& light) const
{
    m_omniDirectionalShadowShader->Bind();
    
    ShadowMap& shadowMap = *light->GetShadowMap();
    
    Window& window = Game::GetInstance().GetWindow();
    glm::ivec2 shadowMapDimensions = shadowMap.GetDimensions();
    window.SetViewPort(shadowMapDimensions.x, shadowMapDimensions.y);

    shadowMap.Write();
    glClear(GL_DEPTH_BUFFER_BIT);

    m_omniDirectionalShadowShader->SetUniform(Uniform<glm::vec3> { "lightPos", light->GetPosition() });
    m_omniDirectionalShadowShader->SetUniform(Uniform<float> { "farPlane", light->GetFarPlane() });

    std::array<glm::mat4, 6> lightTransforms = light->CalculateLightTransforms();
    for (int i = 0; i < lightTransforms.size(); i++)
    {
        std::string name = std::format("lightMatrices[{}]", i);
        m_omniDirectionalShadowShader->SetUniform(Uniform<glm::mat4> { name, lightTransforms[i] });
    }

    RenderScene(*m_omniDirectionalShadowShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_omniDirectionalShadowShader->Unbind();
}

void UseOmniDirectionalShadowMap(Shader& shader, PointLight& light, int index)
{
    std::string name = std::format("omniDirectionalShadowMaps[{}]", index);
    
    const int textureUnit = 3;
    light.GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + index);

    shader.SetUniform(Uniform<int> { name, textureUnit + index });
}

void Scene::RenderPass() const
{
    m_shader->Bind();
    
    std::shared_ptr<Camera> camera = Game::GetInstance().GetCamera().lock();
    m_directionalLight->Use("directionalLight", *m_shader);
    
    m_shader->SetUniform(Uniform<int> { "pointLightCount", (int) m_pointLightCount });
    for (size_t i = 0; i < m_pointLightCount; i++)
    {
        PointLight& light = *m_pointLights[i];
        light.Use(std::format("pointLights[{}]", i), *m_shader);
        UseOmniDirectionalShadowMap(*m_shader, light, i);
    }

    m_shader->SetUniform(Uniform<int> { "spotLightCount", (int) m_spotLightCount });
    for (size_t i = 0; i < m_spotLightCount; i++)
    {
        SpotLight& light = *m_spotLights[i];
        light.Use(std::format("spotLights[{}]", i), *m_shader);
        UseOmniDirectionalShadowMap(*m_shader, light, m_pointLightCount + i);
    }

    glm::mat4 lightTransform = m_directionalLight->CalculateLightTransform();

    m_shader->SetUniform(Uniform<glm::mat4> { "directionalLightTransform", lightTransform });
    m_shader->SetUniform(Uniform<glm::mat4> { "view", camera->GetViewMatrix() });
    m_shader->SetUniform(Uniform<glm::mat4> { "projection", m_projectionMatrix });
    m_shader->SetUniform(Uniform<glm::vec3> { "cameraPos", camera->GetPosition() });

    m_shader->SetUniform(Uniform<int> { "tex", 1 });

    m_directionalLight->GetShadowMap()->Read(GL_TEXTURE2);
    m_shader->SetUniform(Uniform<int> { "directionalShadowMap", 2 });

    RenderScene(*m_shader);

    m_shader->Unbind();
}

void Scene::RenderScene(Shader& shader) const
{
    shader.Validate();
    
    for (const std::shared_ptr<GameObject>& object : m_objects)
    {
        object->Render(shader);
    }
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