#pragma once

#include <string>
#include "GL/glew.h"

class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void Bind() const;
    static void Unbind();
private:
    GLuint m_program;
    
    void AttachShader(GLenum shaderType, const std::string& path);
    void VerifyProgramStatus(GLenum status) const;
};
