#ifndef MESH_H
#define MESH_H

/* TODO : All */

#include "opengl.h"

#include "shader.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	~Mesh();
	void Draw();

private:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	
};

#endif // MESH_H
