#pragma once

#include <array>
#include <string>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>

#include "stb.h"
#include "Shader.h"
#include "Mesh.h"

class SkyBox
{
public:
    SkyBox(std::array<std::string, 6> facePaths);
    ~SkyBox();

    void Render(const glm::mat4& view, const glm::mat4& projection) const;

private:
    std::unique_ptr<Mesh> m_mesh;
    std::unique_ptr<Shader> m_shader;

    GLuint m_texture;
};

