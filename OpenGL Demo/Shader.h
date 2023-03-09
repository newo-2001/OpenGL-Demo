#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "GL/glew.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Shader
{
public:
    static std::unique_ptr<Shader> FromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    Shader(const std::string& vertexSource, const std::string& fragmentSource);
    ~Shader();

    void Bind() const;
    static void Unbind();

    void SetUniform(const std::string& name, const glm::mat4& mat);
    void SetUniform(const std::string& name, const glm::vec3& vec);
    void SetUniform(const std::string& name, float scalar);
    void SetUniform(const std::string& name, int scalar);
private:
    std::unordered_map<std::string, GLint> m_uniformCache = std::unordered_map<std::string, GLint>();
    GLuint m_program;
    
    GLint GetUniformLocation(const std::string& name);
    void AttachShader(GLenum shaderType, const std::string& path);
    void VerifyProgramStatus(GLenum status) const;
};
