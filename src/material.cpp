#include "material.h"


Material::Material() {
}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    checkColors();
}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess) {
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    checkColors();

    if(shininess <= 0.0f)
        m_shininess = 0.01f;
    else
        m_shininess = shininess;
}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat roughness, GLfloat refractionIndex) {
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    checkColors();

    if(roughness <= 0.0f)
        m_roughness = 0.01f;
    else
        m_roughness = roughness;

    if(refractionIndex <= 0.0f)
        m_refractionIndex = 0.01f;
    else
        m_refractionIndex = refractionIndex;
}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat roughness, GLfloat refractionIndex, GLfloat metallic) {
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    checkColors();

    if(roughness <= 0.0f)
        m_roughness = 0.01f;
    else
        m_roughness = roughness;

    if(refractionIndex <= 0.0f)
        m_refractionIndex = 0.01f;
    else
        m_refractionIndex = refractionIndex;

    if(metallic <= 0.0f)
        m_metallic = 0.01f;
    else
        m_metallic = metallic;
}

Material::~Material() {
}

std::string Material::name() const {
	return m_name;
}

void Material::setName(std::string name) {
	m_name = name;
}

glm::vec3 Material::ambient() const {
    return m_ambient;
}

void Material::setAmbient(const glm::vec3 ambient) {
    m_ambient = ambient;
    checkAmbient();
}

glm::vec3 Material::diffuse() const {
    return m_diffuse;
}

void Material::setDiffuse(const glm::vec3 diffuse) {
    m_diffuse = diffuse;
    checkDiffuse();
}

glm::vec3 Material::specular() const {
    return m_specular;
}

void Material::setSpecular(const glm::vec3 specular) {
    m_specular = specular;
    checkSpecular();
}

GLfloat Material::shininess() const {
    return m_shininess;
}

void Material::setShininess(const GLfloat shininess) {
    if(shininess <= 0.0f)
        m_shininess = 0.01f;
    else
        m_shininess = shininess;
}

GLfloat Material::roughness() const {
    return m_roughness;
}

void Material::setRoughness(const GLfloat roughness) {
    if(roughness < 0.0f)
        m_roughness = 0.0f;
    else
        m_roughness = roughness;
}

GLfloat Material::metallic() const {
    return m_metallic;
}

void Material::setMetallic(const GLfloat metallic) {
    if(metallic < 0.0f)
        m_metallic = 0.0f;
    else
        m_metallic = metallic;
}

GLfloat Material::refractionIndex() const {
    return m_refractionIndex;
}

void Material::setRefractionIndex(const GLfloat refractionIndex) {
    if(refractionIndex < 0.0f)
        m_refractionIndex = 0.0f;
    else
        m_refractionIndex = refractionIndex;
}

TextureType Material::textureTypes() const {
	return m_texturesTypes;
}

void Material::updateTextureTypes(TextureType textType) {
	m_texturesTypes |= textType;
}

std::unordered_map<TextureType, std::string> Material::texturesName() const {
	return m_texturesName;
}

void Material::setTexturesName(std::unordered_map<TextureType, std::string> texturesName) {
	m_texturesName = texturesName;
}

std::vector<TextureType> Material::keysTextures() {
	std::vector<TextureType> keys;
	keys.reserve(m_texturesName.size());
	for (auto elem : m_texturesName)
		keys.push_back(elem.first);
	return keys;
}

void Material::addTextureTypeName(TextureType textureType, std::string name) {
	m_texturesTypes |= textureType;
	m_texturesName.insert({ textureType, name });
}

std::unordered_map<std::string, Texture> Material::textures() const {
	return m_textures;
}

void Material::setTextures(std::unordered_map<std::string, Texture> textures) {
	m_textures = textures;
}

void Material::addTexture(std::string name, Texture *texture) {
	m_textures.insert({ name, *texture });
}

void Material::checkAmbient() {
    if(m_ambient.x < 0.0f)
        m_ambient.x = 0.0f;
    if(m_ambient.y < 0.0f)
        m_ambient.y = 0.0f;
    if(m_ambient.z < 0.0f)
        m_ambient.z = 0.0f;
}

void Material::checkDiffuse() {
    if(m_diffuse.x < 0.0f)
        m_diffuse.x = 0.0f;
    if(m_diffuse.y < 0.0f)
        m_diffuse.y = 0.0f;
    if(m_diffuse.z < 0.0f)
        m_diffuse.z = 0.0f;
}

void Material::checkSpecular() {
    if(m_specular.x < 0.0f)
        m_specular.x = 0.0f;
    if(m_specular.y < 0.0f)
        m_specular.y = 0.0f;
    if(m_specular.z < 0.0f)
        m_specular.z = 0.0f;
}

void Material::checkColors() {
    checkAmbient();
    checkDiffuse();
    checkSpecular();
}

void Material::bind(ShaderProgram *shader) {
	GLuint textureCount = 0;
	std::string name;
	if (m_texturesTypes & AMBIENT) {
		glActiveTexture(GL_TEXTURE0 + textureCount);
		shader->setUniform("tex_ambient", textureCount);
		name = m_texturesName.at(AMBIENT).c_str();
		m_textures.at(name).bind();
		++textureCount;
	} else
		shader->setUniform("p_ambient", m_ambient);

	if (m_texturesTypes & DIFFUSE) {
		glActiveTexture(GL_TEXTURE0 + textureCount);
		shader->setUniform("tex_diffuse", textureCount);
		name = m_texturesName.at(DIFFUSE).c_str();
		m_textures.at(name).bind();
		++textureCount;
	} else
		shader->setUniform("p_diffuse", m_diffuse);

	if (m_texturesTypes & SPECULAR) {
		glActiveTexture(GL_TEXTURE0 + textureCount);
		shader->setUniform("tex_specular", textureCount);
		name = m_texturesName.at(SPECULAR).c_str();
		m_textures.at(name).bind();
		++textureCount;
	} else
	    shader->setUniform("p_specular", m_specular);

	if (textureCount > 0)
		glActiveTexture(GL_TEXTURE0);

    shader->setUniform("shininess", m_shininess);
}
