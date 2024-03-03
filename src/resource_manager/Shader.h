#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

// General purpsoe shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility 
// functions for easy management.
class Shader
{
public:
    // State
    GLuint m_ID;
    GLboolean used_;

    // Constructor
    Shader() : m_ID(0), used_(GL_FALSE){ }
    // Sets the current shader as active
    Shader& Use();
    Shader& StopUsing();
    // Compiles the shader from given source code
    void    Compile(const GLchar* vertexSource, const GLchar* fragmentSource); // Note: geometry source code is optional 
    // Utility functions
    void    SetBoolean(const GLchar* name, GLboolean value);
    void    SetFloat(const GLchar* name, GLfloat value);
    void    SetInteger(const GLchar* name, GLint value);
    void    SetVector2f(const GLchar* name, GLfloat x, GLfloat y);
    void    SetVector2f(const GLchar* name, const glm::vec2& value);
    void    SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
    void    SetVector3f(const GLchar* name, const glm::vec3& value);
    void    SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void    SetVector4f(const GLchar* name, const glm::vec4& value);
    void    SetMatrix4(const GLchar* name, const glm::mat4& matrix);
private:
    // Check if uniform exits
    bool    CheckUniform(const GLchar* name);
    // Checks if compilation or linking failed and if so, print the error logs
    void    checkCompileErrors(GLuint object, std::string type);
};


