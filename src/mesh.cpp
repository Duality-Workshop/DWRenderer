#include "mesh.h"

/* TODO */

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices){
	m_vertices = vertices;
	m_indices = indices;
	m_material = new Material();
}

Mesh::~Mesh(){
}

void Mesh::Draw(){
	// TODO
}
