#include "renderer.h"

#include <iostream>

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
	// Object(s)
	m_model = new Model("Resources/Astroboy/astroboy.dae");

	// Light(s)
	m_light = new Light(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(1.2f, 1.0f, 2.0f));

    // Shaders
    m_baseShader = new ShaderProgram("shaders/");
    m_lightShader = new ShaderProgram("shaders/");
        // Simple use of the Shader class
    m_lightShader->createSimpleProgram("light.vert","light.frag");

		// Different use of the Shader class
    ShaderObject* vertexShader = new ShaderObject("shaders/", "base.vert", GL_VERTEX_SHADER);
    ShaderObject* fragmentShader = new ShaderObject("shaders/", "base.frag", GL_FRAGMENT_SHADER);

    vertexShader->load();
	fragmentShader->load(m_model);
    m_baseShader->createProgram();
    m_baseShader->add(vertexShader);
    m_baseShader->add(fragmentShader);
    m_baseShader->link();
}

void Renderer::render(glm::mat4 view, glm::mat4 projection) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw cube
    m_baseShader->useShaderProgram();

    glm::mat4 modelMatrix = glm::mat4();
    this->bindMatrix(m_baseShader, modelMatrix, view, projection);

	// Bind all attributes of the material in the shader
	m_model->material()->bind(m_baseShader);
	// Draw the model
	m_model->Draw();

    // Draw light
	m_light->bind(m_baseShader);
    m_lightShader->useShaderProgram();

    modelMatrix = glm::mat4();
    modelMatrix = glm::translate(modelMatrix, m_light->position());
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    this->bindMatrix(m_lightShader, modelMatrix, view, projection);

    glBindVertexArray(m_lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

}

void Renderer::bindMatrix(ShaderProgram* shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
    glm::mat3 normalMatrix;
    normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
    shader->setUniform("normalMatrix", normalMatrix);
    shader->setUniform("model", model);
    shader->setUniform("view", view);
    shader->setUniform("projection", projection);
}

void Renderer::freeMemory() {
    glDeleteVertexArrays(1, &m_lightVAO);
}

void Renderer::reload() {
    m_baseShader->reload();
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

