#ifndef RENDERER_H
#define RENDERER_H

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

class Renderer
{
public:
    Renderer(GLuint width, GLuint height, Camera *camera);
    ~Renderer();

    void initializeRenderer();
    void render(glm::mat4 view, glm::mat4 projection);
    void bindMatrix(ShaderProgram* shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
    void freeMemory();
    void reload();
    Camera *getCamera() const;
    void setCamera(Camera *camera);
    void setWidth(const GLfloat w);
    void setHeight(const GLfloat h);

private:
    // System
    GLuint m_screenWidth;
    GLuint m_screenHeight;
    Camera *m_camera;

    // Object(s) & Light(s)
    GLuint m_VBO;
    GLuint m_cubeVAO;
    ShaderProgram *m_cubeShader;
    Material* m_cubeMaterial;
    GLuint m_lightVAO;
    ShaderProgram *m_lightShader;
    Light *m_light;
};

#endif // RENDERER_H

