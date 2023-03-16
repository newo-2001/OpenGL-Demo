#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <memory>

#include "GL/glew.h"

template<typename T>
struct Uniform
{
    std::string location;
    T value;
};

class Shader
{
public:
    static std::unique_ptr<Shader> FromFile(const std::string& vertexPath);

    static std::unique_ptr<Shader> FromFiles(const std::string& vertexPath,
                                             const std::string& fragmentPath);

    static std::unique_ptr<Shader> FromFiles(const std::string& vertexPath,
                                             const std::string& fragmentPath,
                                             const std::string& geometryPath);

    Shader(const std::string& vertexSource,
           std::optional<const std::string> fragmentSource = std::nullopt,
           std::optional<const std::string> geometrySource = std::nullopt);
    ~Shader();

    void Bind() const;
    static void Unbind();

    template<typename T>
    void SetUniform(Uniform<T> uniform);

    void Validate() const;
private:
    std::unordered_map<std::string, GLint> m_uniformCache = std::unordered_map<std::string, GLint>();
    GLuint m_program;
    
    GLint GetUniformLocation(const std::string& name);
    void AttachShader(GLenum shaderType, const std::string& path);
    void VerifyProgramStatus(GLenum status) const;
};
