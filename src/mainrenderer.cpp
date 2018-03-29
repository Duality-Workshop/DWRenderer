#include "mainrenderer.h"

#include <iostream>
//#include <random> -- For SSAO
#include <QOpenGLFramebufferObject>

/* TODO : Deffered-shading (for SSAO and other things. 
 * See how to do post-effects in deffered-shading)
 * -- Scene graph
 */

MainRenderer::MainRenderer(GLuint width, GLuint height){
    m_screenWidth = width;
    m_screenHeight = height;
	m_camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f));
	m_camera->setSpeed(5.0f);
	m_camera->setMouseSensitivity(0.5f);
	m_projection = glm::perspective(glm::radians(m_camera->getFov()), (GLfloat)m_screenWidth / (GLfloat)m_screenHeight, 0.1f, 100.0f);
}

MainRenderer::~MainRenderer(){
}

void MainRenderer::initializeRenderer() {
    // Display
    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glEnable(GL_DEPTH_TEST);

	// GLEW
#ifdef _WIN32
	GLenum err = glewInit();
	if (err != GLEW_OK)
		std::cout << "GLEW\t\tErreur" << std::endl;
	else
		std::cout << "GLEW\t\tOK" << std::endl;
#endif

	// Object(s)
	m_model = new Model("Resources/Models/Astroboy/astroboy.dae");
	m_shapes = new Shapes();
	
	// Light(s)
	m_nbLights = 2;
	Light* light1 = new Light(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(3.0f, 1.0f, 5.0f));
	Light* light2 = new Light(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(-3.0f, 1.0f, 5.0f));
	m_lights.push_back(light1);
	m_lights.push_back(light2);

	// Shaders
	m_baseShader = new ShaderProgram("shaders/");
	m_lightShader = new ShaderProgram("shaders/");
	m_HDRShader = new ShaderProgram("shaders/");
	m_blurShader = new ShaderProgram("shaders/");
	//m_SSAOShader = new ShaderProgram("shaders/");
		// Simple use of the Shader class
	m_lightShader->createSimpleProgram("light.vert", "light.frag");
	m_HDRShader->createSimpleProgram("hdr_effects.vert", "hdr_effects.frag");
	m_blurShader->createSimpleProgram("blur.vert", "blur.frag");
	//m_SSAOShader->createSimpleProgram("ssao.vert", "ssao.frag");

		// Different use of the Shader class
	ShaderObject* vertexShader = new ShaderObject("shaders/", "base.vert", GL_VERTEX_SHADER);
	ShaderObject* fragmentShader = new ShaderObject("shaders/", "base.frag", GL_FRAGMENT_SHADER);
	vertexShader->load();
	fragmentShader->load(m_model);
	m_baseShader->createProgram();
	m_baseShader->add(vertexShader);
	m_baseShader->add(fragmentShader);
	m_baseShader->link();

	// Framebuffer - TODO : Create a class for manage Framebuffer
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		// Floating point color buffer
	m_nbColorBuffers = 2;
	m_colorbuffers = createTextures(m_nbColorBuffers, NULL, m_screenWidth, m_screenHeight, GL_TEXTURE_2D,
								GL_FLOAT, GL_RGBA, GL_RGBA16F, GL_NEAREST);
		// Create depth buffer
	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_screenWidth, m_screenHeight);
		// Attach buffers
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	for(int i = 0; i < m_nbColorBuffers; ++i)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorbuffers[i]->getId(), 0);
		// Configure drawBuffers
	GLenum* drawBuffers = new GLenum[m_nbColorBuffers];
	for (int i = 0; i < m_nbColorBuffers; ++i)
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	glDrawBuffers(m_nbColorBuffers, drawBuffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
		// For hdr and effects
	m_wireframe = false;
	m_hdr = false;
	m_exposure = 1.0f;
	m_bloom = false;

		// Blur
	glGenFramebuffers(2, m_pingPongFBO);
	m_pingPongBuffers = createTextures(2, NULL, m_screenWidth, m_screenHeight, GL_TEXTURE_2D, GL_FLOAT, GL_RGBA, GL_RGBA16F, GL_NEAREST);
	for (int i = 0; i < 2; ++i) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_pingPongFBO[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pingPongBuffers[i]->getId(), 0);
		// Also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	m_horizontal = true;
	m_amount = 10;

	delete[] drawBuffers;

	/*// SSAO
		// FBO and colorBuffer
	glGenFramebuffers(1, &m_ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);
	m_ssaoColorBuffer = createTexture(NULL, m_screenWidth, m_screenHeight, 
								GL_TEXTURE_2D, GL_FLOAT, GL_RED, GL_RGB, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoColorBuffer->getId(), 0);
	GLenum drawBuffers2[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers2);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "SSAO Framebuffer not complete!" << std::endl;
		// Sample kernel
	m_nbSamples = 64;
	std::uniform_real_distribution<GLfloat> randomFloats(0.0f, 1.0f);
	std::default_random_engine generator;
	for (int i = 0; i < m_nbSamples; ++i) {
		glm::vec3 sample(
			randomFloats(generator) * 2.0f - 1.0f,
			randomFloats(generator) * 2.0f - 1.0f,
			randomFloats(generator)
		);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		GLfloat scale = GLfloat(i) / m_nbSamples;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		m_ssaoKernel.push_back(sample);
	}

	for (int i = 0; i < 16; ++i) {
		glm::vec3 noise(
			randomFloats(generator) * 2.0f - 1.0f,
			randomFloats(generator) * 2.0f - 1.0f,
			0.0f
		);
		m_ssaoNoise.push_back(noise);
	}
	Texture* noiseTex = createTexture(&m_ssaoNoise[0], 4, 4, GL_TEXTURE_2D, GL_FLOAT, GL_RGB, GL_RGB16F, GL_NEAREST);
	noiseTex->useRepeat();
	m_noiseScale = glm::vec2(GLfloat(m_screenWidth) / 4.0, GLfloat(m_screenHeight) / 4.0);*/
}

void MainRenderer::draw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// 1 - Draw the scene into the floating point framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Matrix
	m_view = m_camera->getViewMatrix();
		// Draw lights
	m_lightShader->useShaderProgram();
	m_modelMatrix = glm::mat4();
	for (int i = 0; i < m_nbLights; ++i) {
		m_modelMatrix = glm::translate(m_modelMatrix, m_lights[i]->position());
		m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(0.2f));
		this->bindMatrix(m_lightShader, m_modelMatrix, m_view, m_projection);
		m_lightShader->setUniform("lightColor", m_lights[i]->diffuse());
		m_shapes->renderCube();
		m_modelMatrix = glm::mat4();
	}

		// SSAO - Don't work in forward-shading (Change for deffered-shading)
	/*glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	m_SSAOShader->useShaderProgram();
	for (int i = 0; i < m_nbSamples; ++i)
		m_SSAOShader->setUniform(("samples[" + std::to_string(i) + "]").c_str(), m_ssaoKernel[i][0]);
	m_SSAOShader->setUniform("projection", projection);
	m_SSAOShader->setUniform("noiseScale", m_noiseScale);
	m_shapes->renderQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

		// Draw model(s)
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    m_baseShader->useShaderProgram();
	for(int i = 0; i < m_nbLights; ++i)
		m_lights[i]->bind(m_baseShader, i);
    m_modelMatrix = glm::mat4();
    this->bindMatrix(m_baseShader, m_modelMatrix, m_view, m_projection);
		// Bind all attributes of the material in the shader
	m_model->material()->bind(m_baseShader);
		// Draw the model
	m_model->Draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Blur the brightness colorBuffer of X amount
	m_blurShader->useShaderProgram();
	for (int i = 0; i < m_amount; ++i) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_pingPongFBO[m_horizontal]);
		m_blurShader->setUniform("horizontal", m_horizontal);
		!i ? m_colorbuffers[1]->bind() : m_pingPongBuffers[!m_horizontal]->bind();
		m_shapes->renderQuad();
		m_horizontal = !m_horizontal;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

// Now render floating point color buffer to 2D quad and tonemap HDR colors 
// to default framebuffer's (clamped) color range - TODO : Change for deffered-shading
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_HDRShader->useShaderProgram();
	m_HDRShader->setUniform("hdrBuffer", 0);
	m_HDRShader->setUniform("bloomBlurBuffer", 1);
	m_HDRShader->setUniform("hdr", m_hdr);
	m_HDRShader->setUniform("exposure", m_exposure);
	m_HDRShader->setUniform("bloom", m_bloom);
	glActiveTexture(GL_TEXTURE0);
	m_colorbuffers[0]->bind();
	glActiveTexture(GL_TEXTURE1);
	m_pingPongBuffers[!m_horizontal]->bind();
	m_shapes->renderQuad();
}

void MainRenderer::bindMatrix(ShaderProgram* shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
    glm::mat3 normalMatrix;
    normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
    shader->setUniform("normalMatrix", normalMatrix);
    shader->setUniform("model", model);
    shader->setUniform("view", view);
    shader->setUniform("projection", projection);
}

// TODO : file for Utils
Texture* MainRenderer::createTexture(void* data, int width, int height, GLenum target,
							GLenum type, GLenum formatImg, GLenum format, GLenum filtering) {
	Texture* tex = new Texture();
	tex->setWidth(width);
	tex->setHeight(height);
	tex->setTarget(target);
	tex->setType(type);
	tex->setFormat(formatImg);
	tex->initTexture2D(data, format, filtering);
	return tex;
}

std::vector<Texture*> MainRenderer::createTextures(int nbTexturesToCreate, void* data, int width, int height, 
									GLenum target, GLenum type, GLenum formatImg, GLenum format, GLenum filtering) {
	std::vector<Texture*> textures;
	Texture* tex_temp;
	for (int i = 0; i < nbTexturesToCreate; ++i) {
		tex_temp = new Texture();
		tex_temp->setWidth(width);
		tex_temp->setHeight(height);
		tex_temp->setTarget(target);
		tex_temp->setType(type);
		tex_temp->setFormat(formatImg);
		tex_temp->initTexture2D(data, format, filtering);
		textures.push_back(tex_temp);
	}
	return textures;
}

GLfloat MainRenderer::lerp(GLfloat a, GLfloat b, GLfloat f) {
	return a + f * (b - a);
}

void MainRenderer::freeMemory() {
	m_shapes->free();
}

void MainRenderer::reload() { // TODO : list of ShaderProgram*
    m_baseShader->reload();
	m_lightShader->reload();
	m_HDRShader->reload();
	m_blurShader->reload();
	//m_SSAOShader->reload();
}

GLboolean MainRenderer::getWireframe() {
	return m_wireframe;
}

void MainRenderer::setWireframe(GLboolean wireframe) {
	m_wireframe = wireframe;
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

Camera *MainRenderer::getCamera() const {
    return m_camera;
}

void MainRenderer::setCamera(Camera *camera) {
    m_camera = camera;
}

void MainRenderer::setWidth(const GLfloat w) {
    m_screenWidth = w;
}

void MainRenderer::setHeight(const GLfloat h) {
    m_screenHeight = h;
}

GLboolean MainRenderer::getHdr() const {
	return m_hdr;
}

void MainRenderer::setHdr(GLboolean hdr) {
	m_hdr = hdr;
}

GLfloat MainRenderer::getExposure() const {
	return m_exposure;
}

void MainRenderer::setExposure(GLfloat exposure) {
	m_exposure = exposure;
}

GLuint MainRenderer::colorBuffers() const {
	return m_nbColorBuffers;
}

void MainRenderer::setColorBuffers(GLuint val) {
	if (val > 14)
		val = 14;
	if (val < 0)
		val = 0;
	m_nbColorBuffers = val;
}

GLboolean MainRenderer::bloom() const {
	return m_bloom;
}

void MainRenderer::setBloom(GLboolean val) {
	m_bloom = val;
}

GLuint MainRenderer::nbLights() const {
	return m_nbLights;
}

void MainRenderer::setNbLights(GLuint val) {
	m_nbLights = val;
}
