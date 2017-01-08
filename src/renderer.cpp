#include "renderer.h"

#include <iostream>
#include <QOpenGLFramebufferObject>

Renderer::Renderer(GLuint width, GLuint height, Camera *camera){
    m_screenWidth = width;
    m_screenHeight = height;
    m_camera = camera;
}

Renderer::~Renderer(){
}

void Renderer::initializeRenderer() {
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

	// Framebuffer
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		// Object(s)
	m_model = new Model("Resources/Astroboy/astroboy.dae");
	m_shapes = new Shapes();
		// Light(s)
	m_nbLights = 2;
	Light* light1 = new Light(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(3.0f, 1.0f, 5.0f));
	Light* light2 = new Light(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(-3.0f, 1.0f, 5.0f));
	m_lights.push_back(light1);
	m_lights.push_back(light2);
		// Floating point color buffer
	m_nbColorBuffers = 2;
	m_colorbuffers = createTextures(m_nbColorBuffers);
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
	m_hdr = false;
	m_exposure = 1.0f;
	m_bloom = false;

		// Blur
	glGenFramebuffers(2, m_pingPongFBO);
	m_pingPongBuffers = createTextures(2);
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

    // Shaders
    m_baseShader = new ShaderProgram("shaders/");
    m_lightShader = new ShaderProgram("shaders/");
	m_hdrShader = new ShaderProgram("shaders/");
	m_blurShader = new ShaderProgram("shaders/");
        // Simple use of the Shader class
    m_lightShader->createSimpleProgram("light.vert","light.frag");
	m_hdrShader->createSimpleProgram("hdr_effects.vert", "hdr_effects.frag");
	m_blurShader->createSimpleProgram("blur.vert", "blur.frag");

		// Different use of the Shader class
    ShaderObject* vertexShader = new ShaderObject("shaders/", "base.vert", GL_VERTEX_SHADER);
    ShaderObject* fragmentShader = new ShaderObject("shaders/", "base.frag", GL_FRAGMENT_SHADER);

    vertexShader->load();
	fragmentShader->load(m_model);
    m_baseShader->createProgram();
    m_baseShader->add(vertexShader);
    m_baseShader->add(fragmentShader);
    m_baseShader->link();

	delete[] drawBuffers;
}

void Renderer::render(glm::mat4 view, glm::mat4 projection) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// 1 - Draw the scene into the floating point framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw lights
	m_lightShader->useShaderProgram();
	glm::mat4 modelMatrix = glm::mat4();
	for (int i = 0; i < m_nbLights; ++i) {
		modelMatrix = glm::translate(modelMatrix, m_lights[i]->position());
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
		this->bindMatrix(m_lightShader, modelMatrix, view, projection);
		m_lightShader->setUniform("lightColor", m_lights[i]->diffuse());
		m_shapes->renderCube();
		modelMatrix = glm::mat4();
	}

		// Draw model(s)
    m_baseShader->useShaderProgram();
	for(int i = 0; i < m_nbLights; ++i)
		m_lights[i]->bind(m_baseShader, i);
    modelMatrix = glm::mat4();
    this->bindMatrix(m_baseShader, modelMatrix, view, projection);
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

// 2 - Now render floating point color buffer to 2D quad and tonemap HDR colors 
// to default framebuffer's (clamped) color range
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_hdrShader->useShaderProgram();
	m_hdrShader->setUniform("hdrBuffer", 0);
	m_hdrShader->setUniform("bloomBlurBuffer", 1);
	m_hdrShader->setUniform("hdr", m_hdr);
	m_hdrShader->setUniform("exposure", m_exposure);
	m_hdrShader->setUniform("bloom", m_bloom);
	glActiveTexture(GL_TEXTURE0);
	m_colorbuffers[0]->bind();
	glActiveTexture(GL_TEXTURE1);
	m_pingPongBuffers[!m_horizontal]->bind();
	m_shapes->renderQuad();
}

void Renderer::bindMatrix(ShaderProgram* shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
    glm::mat3 normalMatrix;
    normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
    shader->setUniform("normalMatrix", normalMatrix);
    shader->setUniform("model", model);
    shader->setUniform("view", view);
    shader->setUniform("projection", projection);
}

std::vector<Texture*> Renderer::createTextures(int nbTexturesToCreate) {
	std::vector<Texture*> textures;
	Texture* tex_temp;
	for (int i = 0; i < nbTexturesToCreate; ++i) {
		tex_temp = new Texture();
		tex_temp->setWidth(m_screenWidth);
		tex_temp->setHeight(m_screenHeight);
		tex_temp->setTarget(GL_TEXTURE_2D);
		tex_temp->setFormat(GL_RGBA);
		tex_temp->initTexture2D(NULL, GL_RGBA16F);
		textures.push_back(tex_temp);
	}
	return textures;
}

void Renderer::freeMemory() {
	m_shapes->free();
}

void Renderer::reload() {
    m_baseShader->reload();
	m_lightShader->reload();
	m_hdrShader->reload();
	m_blurShader->reload();
}

Camera *Renderer::getCamera() const {
    return m_camera;
}

void Renderer::setCamera(Camera *camera) {
    m_camera = camera;
}

void Renderer::setWidth(const GLfloat w) {
    m_screenWidth = w;
}

void Renderer::setHeight(const GLfloat h) {
    m_screenHeight = h;
}

GLboolean Renderer::getHdr() const {
	return m_hdr;
}

void Renderer::setHdr(GLboolean hdr) {
	m_hdr = hdr;
}

GLfloat Renderer::getExposure() const {
	return m_exposure;
}

void Renderer::setExposure(GLfloat exposure) {
	m_exposure = exposure;
}

GLuint Renderer::colorBuffers() const {
	return m_nbColorBuffers;
}

void Renderer::setColorBuffers(GLuint val) {
	if (val > 14)
		val = 14;
	if (val < 0)
		val = 0;
	m_nbColorBuffers = val;
}

GLboolean Renderer::bloom() const {
	return m_bloom;
}

void Renderer::setBloom(GLboolean val) {
	m_bloom = val;
}

GLuint Renderer::nbLights() const {
	return m_nbLights;
}

void Renderer::setNbLights(GLuint val) {
	m_nbLights = val;
}
