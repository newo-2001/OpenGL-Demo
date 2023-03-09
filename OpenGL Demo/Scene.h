#pragma once

#include <vector>

#include <glm/ext/matrix_float4x4.hpp>

#include "Shader.h"
#include "Mesh.h"
#include "Window.h"


class Scene
{
public:
    Scene(const Window& window);

    void Render() const;

    void AddMesh(std::shared_ptr<Mesh>& mesh);
    void UseShader(std::shared_ptr<Shader>& shader);
private:
    glm::mat4 m_projectionMatrix;

    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::shared_ptr<Shader> m_shader;
};
