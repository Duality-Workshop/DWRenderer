#ifndef SHAPES_H
#define SHAPES_H

#include "opengl.h"

class Shapes
{
public:
	Shapes();
	~Shapes();

	void renderQuad();
	void renderCube();
	void free();

private:
	// For 2D quad
	GLuint m_quadVAO;
	GLuint m_quadVBO;

	// For 3D cube
	GLuint m_cubeVAO;
	GLuint m_cubeVBO;
};

#endif // SHAPES_H
