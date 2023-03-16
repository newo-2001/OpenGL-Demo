#include "Shader.h"
#include <sstream>
#include <fstream>
#include <format>

#include <glm/gtc/type_ptr.hpp>

std::string GetActionName(GLenum status);
std::string ReadFile(const std::string& path);
void CompileShader(GLuint shader, GLenum shaderType);

Shader::Shader(const std::string& vertexSource, std::optional<const std::string> fragmentSource,
               std::optional<const std::string> geometrySource)
{
    m_program = glCreateProgram();
    if (!m_program)
    {
        const char* error = (const char*) glewGetErrorString(glGetError());

        throw std::runtime_error(std::format("Failed to create shader program: {}", error));
    }
    
    AttachShader(GL_VERTEX_SHADER, vertexSource);

    if (fragmentSource.has_value())
    {
        AttachShader(GL_FRAGMENT_SHADER, fragmentSource.value());
    }

    if (geometrySource.has_value())
    {
        AttachShader(GL_GEOMETRY_SHADER, geometrySource.value());
    }
    
    try
    {
        glLinkProgram(m_program);
        VerifyProgramStatus(GL_LINK_STATUS);
    }
    catch (...)
    {
        glDeleteProgram(m_program);
        throw;
    }
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

void Shader::Validate() const
{
    glValidateProgram(m_program);
    VerifyProgramStatus(GL_VALIDATE_STATUS);
}

void Shader::Bind() const
{
    glUseProgram(m_program);
}

void Shader::Unbind()
{
    glUseProgram(0);
}

void Shader::VerifyProgramStatus(GLenum status) const
{
    GLint result;
    glGetProgramiv(m_program, status, &result);
    if (result) return;

    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &result);

    std::unique_ptr<GLchar[]> errorLog = std::make_unique<GLchar[]>(result);
    glGetProgramInfoLog(m_program, result, NULL, errorLog.get());

    std::string action = GetActionName(status);
    std::string message = std::format("Failed to {} shader:\n\t{}", action, errorLog.get());

    throw std::runtime_error(message);
}

void Shader::AttachShader(GLenum shaderType, const std::string& source)
{
    const GLchar* sources[] = { source.c_str() };
    const GLint sourceLengths[] = { (GLint) source.size() };

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, sources, sourceLengths);

    try
    {
        CompileShader(shader, shaderType);
    }
    catch (...)
    {
        glDeleteShader(shader);
        throw;
    }
    
    glAttachShader(m_program, shader);
    glDeleteShader(shader);
}

GLint Shader::GetUniformLocation(const std::string& name) {
    auto it = m_uniformCache.find(name);
    
    // Cache miss
    if (it == m_uniformCache.end())
    {
        GLint location = glGetUniformLocation(m_program, name.c_str());
        /*if (location == -1)
        {
            throw std::runtime_error(std::format("Could not find uniform variable '{}'", name));
        }*/

        m_uniformCache.insert({ name, location });
        return location;
    }

    return (*it).second;
}

std::unique_ptr<Shader> Shader::FromFile(const std::string& vertexPath)
{
    std::string vertexSource = ReadFile(vertexPath);

    return std::make_unique<Shader>(vertexSource, std::nullopt);
}

std::unique_ptr<Shader> Shader::FromFiles(const std::string& vertexPath,
                                          const std::string& fragmentPath)
{
    std::string vertexSource = ReadFile(vertexPath);
    std::string fragmentSource = ReadFile(fragmentPath);

    return std::make_unique<Shader>(vertexSource, std::make_optional(fragmentSource));
}

std::unique_ptr<Shader> Shader::FromFiles(const std::string& vertexPath,
                                          const std::string& fragmentPath,
                                          const std::string& geometryPath)
{
    std::string vertexSource = ReadFile(vertexPath);
    std::string fragmentSource = ReadFile(fragmentPath);
    std::string geometrySource = ReadFile(geometryPath);

    return std::make_unique<Shader>(vertexSource, std::make_optional(fragmentSource),
                                    std::make_optional(geometrySource));
}

std::string ReadFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error(std::format("Failed to open file: {}", path));
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string GetActionName(GLenum status)
{
    switch (status)
    {
        case GL_LINK_STATUS:
            return "link";
        case GL_VALIDATE_STATUS:
            return "verify";
        default:
            return "perform an unknown action on a";
    }
}

void CompileShader(GLuint shader, GLenum shaderType)
{
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result) return;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);

    std::unique_ptr<GLchar[]> errorLog = std::make_unique<GLchar[]>(result);
    glGetShaderInfoLog(shader, result, NULL, errorLog.get());
    
    std::string message = std::format("Failed to compile shader: {}", errorLog.get());
    throw std::runtime_error(message);
}

template<>
void Shader::SetUniform<glm::mat4>(Uniform<glm::mat4> uniform)
{
    GLint location = GetUniformLocation(uniform.location);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(uniform.value));
}

template<>
void Shader::SetUniform<glm::vec3>(Uniform<glm::vec3> uniform)
{
    GLint location = GetUniformLocation(uniform.location);
    glUniform3fv(location, 1, glm::value_ptr(uniform.value));
}

template<>
void Shader::SetUniform<float>(Uniform<float> uniform)
{
    GLint location = GetUniformLocation(uniform.location);
    glUniform1f(location, uniform.value);
}

template<>
void Shader::SetUniform<int>(Uniform<int> uniform)
{
    GLint location = GetUniformLocation(uniform.location);
    glUniform1i(location, uniform.value);
}
