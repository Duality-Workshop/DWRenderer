#ifndef MESH_H
#define MESH_H

/* TODO : All */

#include "opengl.h"

#include "shader.h"

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
	Mesh(vector<Vertex> vertices, vector<GLuint> indices);
	~Mesh();
	void Draw();

private:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	
};

#endif // MESH_H
