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

    if(roughness < 0.0f)
        m_roughness = 0.0f;
    else
        m_roughness = roughness;

    if(refractionIndex < 0.0f)
        m_refractionIndex = 0.0f;
    else
        m_refractionIndex = refractionIndex;
}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLfloat roughness, GLfloat refractionIndex, GLfloat metallic) {
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    checkColors();

    if(roughness < 0.0f)
        m_roughness = 0.0f;
    else
        m_roughness = roughness;

    if(refractionIndex < 0.0f)
        m_refractionIndex = 0.0f;
    else
        m_refractionIndex = refractionIndex;

    if(metallic < 0.0f)
        m_metallic = 0.0f;
    else
        m_metallic = metallic;
}

Material::~Material() {
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
    shader->setUniform("material.ambient", m_ambient);
    shader->setUniform("material.diffuse", m_diffuse);
    shader->setUniform("material.specular", m_specular);
    shader->setUniform("material.shininess", m_shininess);
}
