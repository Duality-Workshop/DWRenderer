#ifndef LIGHT_H
#define LIGHT_H

#include "shader.h"

class Light
{
public:
	Light();
	Light(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient);
	Light(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, glm::vec3 position);
	~Light();

	glm::vec3 position() const;
	void setPosition(glm::vec3 val);
	glm::vec3 diffuse() const;
	void setDiffuse(glm::vec3 val);
	glm::vec3 specular() const;
	void setSpecular(glm::vec3 val);
	glm::vec3 ambient() const;
	void setAmbient(glm::vec3 val);

	void bind(ShaderProgram *shader);

private:
	glm::vec3 m_position;
	glm::vec3 m_diffuse;
	glm::vec3 m_specular;
	glm::vec3 m_ambient;
};

#endif // LIGHT_H
