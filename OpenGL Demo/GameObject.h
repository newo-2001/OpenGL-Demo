#pragma once

#include <memory>

#include <glm/ext/matrix_float4x4.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"

class GameObject
{
public:
    GameObject(std::shared_ptr<Renderable>& renderable, glm::mat4 transform = glm::mat4(1.0f),
               std::shared_ptr<Texture> texture = {}, std::shared_ptr<Material> material = {});

    void Render(Shader& shader);

    void SetTransform(glm::mat4 transform) { m_transform = transform; }
private:
    std::shared_ptr<Renderable> m_renderable;
    std::shared_ptr<Texture> m_texture;
    std::shared_ptr<Material> m_material;
    glm::mat4 m_transform;
};

