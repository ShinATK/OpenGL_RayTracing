#pragma once

#include "Texture.h"
#include "Shader.h"

#include <map>
#include <string>
#include <vector>

#include <iostream>
#include <sstream>
#include <fstream>

// A static singleton ResourceManager class
class ResourceManager
{
public:

    // Resource storage
    static std::map<std::string, Shader>    m_shaders;
    static std::map<std::string, Texture2D> m_textures;

    // Retrieves a stored shader/texture
    static Shader&      GetShader(std::string name);
    static Texture2D&   GetTexture(std::string name);

    // Load shader/texture
    static Shader       LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, std::string name);
    static Texture2D    LoadTexture(const GLchar* file, GLboolean alpha, std::string name);
    static Texture2D    LoadCubeMap(std::vector< std::string>& faces, GLboolean alpha, std::string name);

    static void UpdateShader(glm::mat4& projection, glm::mat4& view, glm::vec3& viewPos);

    // Check Exist
    bool CheckShader( std::string name ) {
        return m_shaders.find( name ) != m_shaders.end();
    }
    bool CheckTexture( std::string name ) {
        return m_textures.find( name ) != m_textures.end();
    }

    // Clear Up
    static void Clear();
private:
    // Private constructor
    ResourceManager() {};
    // Loads and generates a shader from file
    static Shader       loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile);
    // Loads a single texture from file
    static Texture2D    loadTextureFromFile(const GLchar* file, GLboolean alpha);
    static Texture2D    loadCubeMapFromFile(std::vector< std::string>& faces, GLboolean alpha);
};

