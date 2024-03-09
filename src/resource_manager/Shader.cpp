#include "Shader.h"

Shader& Shader::Use()
{
    glUseProgram(this->m_ID);
    this->used_ = GL_TRUE;
    return *this;
}

Shader& Shader::StopUsing()
{
    glUseProgram(0);
    this->used_ = GL_FALSE;
    return *this;
}

void Shader::Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource)
{
    GLuint sVertex, sFragment, gShader;
    // Vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");
    // Fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");
    // Geometry Shader
    if (geometrySource != nullptr)
    {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, NULL);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }
    // Shader Program
    this->m_ID = glCreateProgram();
    glAttachShader(this->m_ID, sVertex);
    glAttachShader(this->m_ID, sFragment);
    if (geometrySource != nullptr)
        glAttachShader(this->m_ID, gShader);
    glLinkProgram(this->m_ID);
    checkCompileErrors(this->m_ID, "PROGRAM");
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource != nullptr)
        glDeleteShader(gShader);
}

void Shader::SetBoolean(const GLchar* name, GLboolean value)
{
    this->Use();
    if (CheckUniform(name)) glUniform1i(glGetUniformLocation(this->m_ID, name), value);
    this->StopUsing();
}
void Shader::SetFloat(const GLchar* name, GLfloat value)
{
    this->Use();
    if (CheckUniform(name)) glUniform1f(glGetUniformLocation(this->m_ID, name), value);
    this->StopUsing();
}
void Shader::SetInteger(const GLchar* name, GLint value)
{
    this->Use();
    if (CheckUniform(name)) glUniform1i(glGetUniformLocation(this->m_ID, name), value);
    this->StopUsing();
}
void Shader::SetVector2f(const GLchar* name, GLfloat x, GLfloat y)
{
    this->Use();
    if (CheckUniform(name)) glUniform2f(glGetUniformLocation(this->m_ID, name), x, y);
    this->StopUsing();
}
void Shader::SetVector2f(const GLchar* name, const glm::vec2& value)
{
    this->Use();
    if (CheckUniform(name)) glUniform2f(glGetUniformLocation(this->m_ID, name), value.x, value.y);
    this->StopUsing();
}
void Shader::SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z)
{
    this->Use();
    if (CheckUniform(name)) glUniform3f(glGetUniformLocation(this->m_ID, name), x, y, z);
    this->StopUsing();
}
void Shader::SetVector3f(const GLchar* name, const glm::vec3& value)
{
    this->Use();
    if (CheckUniform(name)) glUniform3f(glGetUniformLocation(this->m_ID, name), value.x, value.y, value.z);
    this->StopUsing();
}
void Shader::SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    this->Use();
    if (CheckUniform(name)) glUniform4f(glGetUniformLocation(this->m_ID, name), x, y, z, w);
    this->StopUsing();
}
void Shader::SetVector4f(const GLchar* name, const glm::vec4& value)
{
    this->Use();
    if (CheckUniform(name)) glUniform4f(glGetUniformLocation(this->m_ID, name), value.x, value.y, value.z, value.w);
    this->StopUsing();
}
void Shader::SetMatrix4(const GLchar* name, const glm::mat4& matrix)
{
    this->Use();
    if (CheckUniform(name)) glUniformMatrix4fv(glGetUniformLocation(this->m_ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
    this->StopUsing();
}


bool Shader::CheckUniform(const GLchar* name) {
    GLint location = glGetUniformLocation(this->m_ID, name);
    if (location == -1) {
        // std::cerr << "Uniform '" << name << "' does not exist in the shader." << std::endl;
        return false;
    }
    return true;
}

void Shader::checkCompileErrors(GLuint object, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}


