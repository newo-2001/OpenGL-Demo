#include "Renderable.h"
#include "Shader.h"

Renderable::Renderable(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Texture>& texture,
                       std::shared_ptr<Material>& material, glm::mat4 transform) :
    m_mesh(mesh),
    m_transform(transform),
    m_texture(texture),
    m_material(material)
{ }

void Renderable::Render(Shader& shader)
{
    shader.SetUniform(Uniform<glm::mat4> { "model", m_transform });
    
    if (m_texture)
    {
        m_texture->Use();
    }

    if (m_material)
    {
        m_material->Use(shader);
    }

    m_mesh->Render();
}
