#pragma once

#include "Texture.h"
#include "Shader.h"
#include "Material.h"

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
    static std::map<std::string, Material>  m_materials;

    // Retrieves a stored shader/texture
    static Material&    GetMaterial(std::string name);
    static Shader&      GetShader(std::string name);
    static Texture2D&   GetTexture(std::string name);

    // Load shader/texture
    static Material     LoadMaterial(const GLchar* name, const GLchar* category, const GLchar* file);
    static Shader       LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, std::string name, const GLchar* gShaderFile=nullptr);
    static Texture2D    LoadTexture(const GLchar* file, GLboolean alpha, std::string name);
    static Texture2D    LoadCubeMap(std::vector< std::string>& faces, GLboolean alpha, std::string name);

    // Check Exist
    static bool CheckMaterial(std::string name);
    static bool CheckShader(std::string name);
    static bool CheckTexture(std::string name);

    // Configure Shader
    static void         UpdateShader(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos);


    // Clear Up
    static void          Clear();
private:
    // Private constructor
    ResourceManager() {};
    // Loads and generates a shader from file
    static Shader       loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
    // Loads a single texture from file
    static Texture2D    loadTextureFromFile(const GLchar* file, GLboolean alpha);
    static Texture2D    loadCubeMapFromFile(std::vector< std::string>& faces, GLboolean alpha);
};

