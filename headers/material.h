#ifndef MATERIAL_H
#define MATERIAL_H

#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>

#include <glm/glm.hpp>

#include "shader.h"

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

    void checkAmbient();
    void checkDiffuse();
    void checkSpecular();
    void checkColors();
    void bind(ShaderProgram *shader);

private:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    GLfloat m_shininess;
    GLfloat m_roughness;
    GLfloat m_metallic;
    GLfloat m_refractionIndex;
};

#endif // MATERIAL_H
