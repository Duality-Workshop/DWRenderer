#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <unordered_map>

#include "shader.h"

typedef char TextureTypes;

enum TextureType : TextureTypes {
	AMBIENT = 0x01,
	DIFFUSE = 0x02,
	SPECULAR = 0x04,
	NORMAL = 0x08
};

const std::unordered_map<TextureType, std::string> textureTypeString({
	{ AMBIENT, "Ambient" },
	{ DIFFUSE, "Diffuse" },
	{ SPECULAR, "Specular" },
	{ NORMAL, "Normal" }
});

class Material
{
public:
    Material();
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess);
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat roughness, GLfloat refractionIndex);
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat roughness, GLfloat refractionIndex, GLfloat metallic);
    ~Material();

    glm::vec3 ambient() const;
    void setAmbient(const glm::vec3 ambient);
    glm::vec3 diffuse() const;
    void setDiffuse(const glm::vec3 diffuse);
    glm::vec3 specular() const;
    void setSpecular(const glm::vec3 specular);
    GLfloat shininess() const;
    void setShininess(const GLfloat shininess);
    GLfloat roughness() const;
    void setRoughness(const GLfloat roughness);
    GLfloat metallic() const;
    void setMetallic(const GLfloat metallic);
    GLfloat refractionIndex() const;
    void setRefractionIndex(const GLfloat refractionIndex);

	TextureTypes textureTypes() const;
	std::unordered_map<TextureType, std::string> textures() const;
	void setTextures(std::unordered_map<TextureType, std::string> textures);
	void addTexture(TextureType textureType, std::string name);

    void checkAmbient();
    void checkDiffuse();
    void checkSpecular();
    void checkColors();
    
	void bind(ShaderProgram *shader);

private:
	TextureTypes m_texturesTypes = 0x00;
	std::unordered_map<TextureType, std::string> m_textures;

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    GLfloat m_shininess;
    GLfloat m_roughness;
    GLfloat m_metallic;
    GLfloat m_refractionIndex;
};

#endif // MATERIAL_H
