#pragma once

#include <memory>

#include <glm/ext/matrix_float4x4.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

class Renderable
{
public:
    Renderable(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Texture>& texture,
               std::shared_ptr<Material>& material, glm::mat4 transform = glm::mat4(1.0f));
    
    void Render(Shader& shader);
private:
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Texture> m_texture;
    std::shared_ptr<Material> m_material;
    glm::mat4 m_transform;
};

