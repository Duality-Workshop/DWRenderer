#include "light.h"
#include "..\headers\light.h"

Light::Light() {
}

Light::Light(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient) {
	m_diffuse = diffuse;
	m_specular = specular;
	m_ambient = ambient;
}

Light::Light(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, glm::vec3 position) {
	m_diffuse = diffuse;
	m_specular = specular;
	m_ambient = ambient;
	m_position = position;
}

Light::~Light() {
}

glm::vec3 Light::position() const {
	return m_position;
}

void Light::setPosition(glm::vec3 val) {
	m_position = val;
}

glm::vec3 Light::diffuse() const {
	return m_diffuse;
}

void Light::setDiffuse(glm::vec3 val) {
	m_diffuse = val;
}

glm::vec3 Light::specular() const {
	return m_specular;
}

void Light::setSpecular(glm::vec3 val) {
	m_specular = val;
}

glm::vec3 Light::ambient() const {
	return m_ambient;
}

void Light::setAmbient(glm::vec3 val) {
	m_ambient = val;
}

void Light::bind(ShaderProgram *shader) {
	shader->setUniform("light.position", m_position);
	shader->setUniform("light.ambient", m_ambient);
	shader->setUniform("light.diffuse", m_diffuse);
	shader->setUniform("light.specular", m_specular);
}

