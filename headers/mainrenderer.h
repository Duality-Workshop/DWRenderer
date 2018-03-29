#ifndef MAINRENDERER_H
#define MAINRENDERER_H

#include "opengl.h"

#include <QTime>

// GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "material.h"
#include "light.h"
#include "model.h"
#include "shapes.h"

class MainRenderer
{
public:
    MainRenderer(GLuint width, GLuint height);
    ~MainRenderer();

    void initializeRenderer();
    void draw();
    void bindMatrix(ShaderProgram* shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	// TODO : create file Utils
	Texture* createTexture(void* data, int width, int height, GLenum target, GLenum type, GLenum formatImg, GLenum format, GLenum filtering);
	std::vector<Texture*> MainRenderer::createTextures(int nbTexturesToCreate, void* data, int width, int height, 
												GLenum target, GLenum type, GLenum formatImg, GLenum format, GLenum filtering);
	GLfloat lerp(GLfloat a, GLfloat b, GLfloat f);
    void freeMemory();
    void reload();
	GLboolean getWireframe();
	void setWireframe(GLboolean wireframe);
    Camera *getCamera() const;
    void setCamera(Camera *camera);
    void setWidth(const GLfloat w);
    void setHeight(const GLfloat h);
	GLboolean getHdr() const;
	void setHdr(GLboolean hdr);
	GLfloat getExposure() const;
	void setExposure(GLfloat exposure);
	GLuint colorBuffers() const;
	void setColorBuffers(GLuint val);
	GLboolean bloom() const;
	void setBloom(GLboolean val);
	GLuint nbLights() const;
	void setNbLights(GLuint val);

private:
    // System
    GLuint m_screenWidth;
    GLuint m_screenHeight;
    Camera *m_camera;

	// Matrix
	glm::mat4 m_view;
	glm::mat4 m_projection;
	glm::mat4 m_modelMatrix;

	// For HDR and effects
		// Wireframe
	GLboolean m_wireframe;
		// HDR
	ShaderProgram *m_HDRShader;
	GLuint m_nbColorBuffers;
	std::vector<Texture*> m_colorbuffers;
	GLboolean m_hdr;
	GLfloat m_exposure;
		// Bloom
	ShaderProgram *m_blurShader;
	GLboolean m_bloom;
	GLboolean m_horizontal;
	GLuint m_amount;
	GLuint m_pingPongFBO[2];
	std::vector<Texture*> m_pingPongBuffers;
	/*	// SSAO
	ShaderProgram *m_SSAOShader;
	GLuint m_ssaoFBO;
	Texture *m_ssaoColorBuffer;
	std::vector<glm::vec3> m_ssaoKernel;
	GLuint m_nbSamples;
	std::vector<glm::vec3> m_ssaoNoise;
	glm::vec2 m_noiseScale;*/

    // For the scene
	Shapes *m_shapes;
	GLuint m_FBO;
    ShaderProgram *m_baseShader;
	Model *m_model;

    ShaderProgram *m_lightShader;
	GLuint m_nbLights;
    std::vector<Light*> m_lights;
};

#endif // MAINRENDERER_H

