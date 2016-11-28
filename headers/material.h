#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <unordered_map>

#include "shader.h"
#include "texture.h"

/* Need some refactoring */

typedef char TextureType;

enum TextureTypes : TextureType {
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

	std::string name() const;
	void setName(std::string name);
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

	TextureType textureTypes() const;
	void updateTextureTypes(TextureType textType);
	std::unordered_map<TextureType, std::string> texturesName() const;
	void setTexturesName(std::unordered_map<TextureType, std::string> texturesName);
	std::vector<TextureType> keysTextures();
	void addTextureTypeName(TextureType textureType, std::string name);
	std::unordered_map<std::string, Texture> textures() const;
	void setTextures(std::unordered_map<std::string, Texture> textures);
	void addTexture(std::string name, Texture *texture);


    void checkAmbient();
    void checkDiffuse();
    void checkSpecular();
    void checkColors();
    
	void bind(ShaderProgram *shader);

private:
	TextureType m_texturesTypes = 0x00;
	std::unordered_map<TextureType, std::string> m_texturesName;
	std::unordered_map<std::string, Texture> m_textures;

	std::string m_name;
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    GLfloat m_shininess;
    GLfloat m_roughness;
    GLfloat m_metallic;
    GLfloat m_refractionIndex;
};

#endif // MATERIAL_H
