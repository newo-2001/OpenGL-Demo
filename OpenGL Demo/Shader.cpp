#include "Shader.h"
#include <sstream>
#include <fstream>
#include <format>
#include <memory>

std::string getActionName(GLenum status);
std::string readFile(const std::string& path);
void compileShader(GLuint shader, GLenum shaderType);

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    m_program = glCreateProgram();
    if (!m_program)
    {
        throw std::runtime_error("Failed to create shader program");
    }
    
    attachShader(GL_VERTEX_SHADER, vertexPath);
    attachShader(GL_FRAGMENT_SHADER, fragmentPath);
    
    try
    {
        link();
    }
    catch (std::exception&)
    {
        glDeleteProgram(m_program);
        throw;
    }
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

void Shader::bind() const
{
    glUseProgram(m_program);
}

void Shader::unbind()
{
    glUseProgram(0);
}

void Shader::link()
{
    glLinkProgram(m_program);

    verifyProgramStatus(GL_LINK_STATUS);
}

void Shader::verifyProgramStatus(GLenum status) const
{
    GLint result;
    glGetProgramiv(m_program, status, &result);
    if (result) return;

    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &result);

    std::unique_ptr<GLchar[]> errorLog = std::make_unique<GLchar[]>(result);
    glGetProgramInfoLog(m_program, result, NULL, errorLog.get());

    std::string action = getActionName(status);
    std::string message = std::format("Failed to {} shader:\n\t{}", action, errorLog.get());

    throw std::runtime_error(message);
}

void Shader::attachShader(GLenum shaderType, const std::string& filepath)
{
    std::string source = readFile(filepath);
    const GLchar* sources[] = { source.c_str() };
    const GLint sourceLengths[] = { source.size() };

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, sources, sourceLengths);

    try
    {
        compileShader(shader, shaderType);
    }
    catch (...)
    {
        glDeleteShader(shader);
        std::throw_with_nested(std::runtime_error(std::format("Failed to compile shader: '{}'", filepath)));
    }
    
    glAttachShader(m_program, shader);
    glDeleteShader(shader);
}

std::string readFile(const std::string& path)
{
    std::ifstream file(path);
    std::stringstream buffer;

    buffer << file.rdbuf();
    return buffer.str();
}

std::string getActionName(GLenum status)
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

void compileShader(GLuint shader, GLenum shaderType)
{
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result) return;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);

    std::unique_ptr<GLchar[]> errorLog = std::make_unique<GLchar[]>(result);
    glGetShaderInfoLog(shader, result, NULL, errorLog.get());
    
    throw std::runtime_error(errorLog.get());
}
