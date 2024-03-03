#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <glm/glm.hpp>

#include "Shader.h"

class Material {
private:
	std::string	m_category;

	glm::vec3			m_ambient  = glm::vec3(1.0f);
	glm::vec3			m_diffuse  = glm::vec3(1.0f);
	glm::vec3			m_specular = glm::vec3(0.5f);

	float				m_shininess  = 1.0f;

public:
    Material() {}
	Material(const std::string& category, const std::string& filePath) : m_category(category) {
		this->LoadMaterial(filePath);
        std::cout << "Category: " << m_category << std::endl;
        std::cout << "Ambient: (" << m_ambient.x << ", " << m_ambient.y << ", " << m_ambient.z << ")" << std::endl;
        std::cout << "Diffuse: (" << m_diffuse.x << ", " << m_diffuse.y << ", " << m_diffuse.z << ")" << std::endl;
        std::cout << "Specular: (" << m_specular.x << ", " << m_specular.y << ", " << m_specular.z << ")" << std::endl;
        std::cout << "Shininess: " << m_shininess << std::endl;
	}

    // Update Material with a new file
    void UpdateMaterial(const std::string& filePath) {
        this->LoadMaterial(filePath);
    }

    // Update shader
    void UpdateShader(Shader& shader) const {
        shader.SetVector3f("material.ambient", this->m_ambient);
        shader.SetVector3f("material.diffuse", this->m_diffuse);
        shader.SetVector3f("material.specular", this->m_specular);
        shader.SetFloat("material.shininess", this->m_shininess);
    }

    // Set / Get
    std::string GetCategory() const { return this->m_category; }

private:
	// Load Material from file
	void LoadMaterial(const std::string& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string category;
            float x, y, z;
            if (iss >> category) {
                if (category == "shininess") {
                    if (iss >> x) m_shininess = x;
                }
                else if (iss >> x >> y >> z) {
                    if (category == "ambient")
                        m_ambient = glm::vec3(x, y, z);
                    else if (category == "diffuse")
                        m_diffuse = glm::vec3(x, y, z);
                    else if (category == "specular")
                        m_specular = glm::vec3(x, y, z);
                }
            }
            else {
                std::cerr << "Error reading file in '" << filePath << "'." << std::endl;
            }
        }

        file.close();
	}
};

