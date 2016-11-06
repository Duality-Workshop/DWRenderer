#ifndef OPENGL_H
#define OPENGL_H

#ifdef __linux__
	#define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
#endif

#ifdef _WIN32
	//#define GLEW_STATIC
	#include <GL/glew.h>
#endif

#endif // OPENGL_H
