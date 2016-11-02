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

    // Shaders
    m_cubeShader = new ShaderProgram("shaders/");
    m_lightShader = new ShaderProgram("shaders/");
        // Simple use of the Shader class
    m_cubeShader->createSimpleProgram("cube.vert", "cube.frag");
    m_lightShader->createSimpleProgram("light.vert","light.frag");

    /*// Different use of the Shader class
    ShaderObject* vertexShader = new ShaderObject("../shaders/", "cube.vert", GL_VERTEX_SHADER);
    ShaderObject* fragmentShader = new ShaderObject("../shaders/", "cube.frag", GL_FRAGMENT_SHADER);

    vertexShader->load();
    fragmentShader->load();

    m_cubeShader->createProgram();

    m_cubeShader->add(vertexShader);
    m_cubeShader->add(fragmentShader);

    m_cubeShader->link();*/

    // Initialize vertices for the quad
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // Object(s)
    glGenVertexArrays(1, &m_cubeVAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // Material
    m_cubeMaterial = new Material(glm::vec3(0.1745f, 0.01175f, 0.01175f), // Ruby here
                                  glm::vec3(0.61424f, 0.04136f, 0.04136f),
                                  glm::vec3(0.727811f, 0.626959f, 0.626959f), 0.6f);

    // Ligth(s)
    glGenVertexArrays(1, &m_lightVAO);
    glBindVertexArray(m_lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    m_lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
}

void Renderer::render(glm::mat4 view, glm::mat4 projection) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw cube
    m_cubeShader->useShaderProgram();

    glm::mat4 model = glm::mat4();
    this->bindMatrix(m_cubeShader, model, view, projection);

    m_cubeMaterial->bind(m_cubeShader);
    m_cubeShader->setUniform("viewPos", m_camera->getPosition());
    // TODO : class Light
    m_cubeShader->setUniform("light.position", m_lightPos);
    m_cubeShader->setUniform("light.ambient", glm::vec3(0.1f,0.1f,0.1f));
    m_cubeShader->setUniform("light.diffuse", glm::vec3(0.5f,0.5f,0.5f));
    m_cubeShader->setUniform("light.specular", glm::vec3(1.1f,1.0f,1.0f));

    glBindVertexArray(m_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // Draw light
    m_lightShader->useShaderProgram();

    model = glm::mat4();
    model = glm::translate(model, m_lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    this->bindMatrix(m_lightShader, model, view, projection);

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
    glDeleteVertexArrays(1, &m_cubeVAO);
    glDeleteVertexArrays(1, &m_lightVAO);
    glDeleteBuffers(1, &m_VBO);
}

void Renderer::reload() {
    m_cubeShader->reload();
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

