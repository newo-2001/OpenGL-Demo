#pragma once

#include <string>
#include "GL/glew.h"

class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void bind() const;
    static void unbind();
private:
    GLuint m_program;
    
    void attachShader(GLenum shaderType, const std::string& path);
    void verifyProgramStatus(GLenum status) const;
    void link();
};
