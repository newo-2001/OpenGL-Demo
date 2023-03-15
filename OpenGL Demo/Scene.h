#pragma once

#include <array>

#include <glm/ext/matrix_float4x4.hpp>

#include "Shader.h"
#include "Window.h"
#include "LightSource.h"
#include "Renderable.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "GameObject.h"

class Scene
{
public:
    const static size_t MAX_POINT_LIGHTS = 3;
    const static size_t MAX_SPOT_LIGHTS = 3;

    Scene(const Window& window);

    void Render() const;

    void AddObject(std::shared_ptr<GameObject> object);

    void UseShader(std::shared_ptr<Shader>& shader);

    void AddDirectionalLight(std::unique_ptr<DirectionalLight>& light);
    void AddPointLight(std::unique_ptr<PointLight>& pointLight);
    void AddSpotLight(std::unique_ptr<SpotLight>& spotLight);

    std::array<std::unique_ptr<SpotLight>, MAX_SPOT_LIGHTS>& GetSpotLights() { return m_spotLights; }
private:
    glm::mat4 m_projectionMatrix;
    std::shared_ptr<Shader> m_shader;

    std::unique_ptr<Shader> m_directionalShadowShader;
    std::unique_ptr<DirectionalLight> m_directionalLight;

    std::vector<std::shared_ptr<GameObject>> m_objects;

    std::array<std::unique_ptr<PointLight>, Scene::MAX_POINT_LIGHTS> m_pointLights;
    std::array<std::unique_ptr<SpotLight>, Scene::MAX_SPOT_LIGHTS> m_spotLights;
    
    size_t m_pointLightCount = 0;
    size_t m_spotLightCount = 0;
    
    void RenderScene(Shader& shader) const;
    void RenderPass() const;
    void DirectionalShadowMapPass(const DirectionalLight& light) const;
};
