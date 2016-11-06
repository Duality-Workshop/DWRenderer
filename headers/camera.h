#ifndef CAMERA_H
#define CAMERA_H

#define GL_GLEXT_PROTOTYPES
#ifdef _WIN32
	#include <GL/glew.h>
#endif

// GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    typedef enum {
        NONE,
        FORWARD,
        BACKWARD,
        RIGHT,
        LEFT,
        FORWARDLEFT,
        FORWARDRIGHT,
        BACKWARDLEFT,
        BACKWARDRIGHT,
        UP,
        DOWN,
        YAWLEFT,
        YAWRIGHT,
        PITCHUP,
        PITCHDOWN
    } Direction;

    typedef enum {
        KEY_FORWARD,
        KEY_BACKWARD,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_UP,
        KEY_DOWN,
        KEY_PITCHUP,
        KEY_PITCHDOWN,
        KEY_YAWLEFT,
        KEY_YAWRIGHT
    } Key;

    Camera(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f), glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f), GLfloat fov = 45.0f, GLfloat yaw = -90.0f, GLfloat pitch = 0.0f);
    ~Camera();

    glm::mat4 getViewMatrix() const;
    GLfloat getFov() const;
    glm::vec3 getPosition() const;
    void setSpeed(const GLfloat speed);
    void setMouseSensitivity(const GLfloat mouseSensitivity);
    void resetPitch();
    void resetYaw();
    void initCamera();
    void updateCamera(GLfloat deltaTime);
    void processKeyPress(Key key);
    void processKeyRelease(Key key);
    void beginMouseMove();
    void endMouseMove();
    void processMouseMovement(GLfloat xoffset, GLfloat yoffset);
    void processMouseScroll(GLfloat yoffset);

private:
    glm::vec3 m_position;
    glm::vec3 m_position_init;
    glm::vec3 m_target;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    glm::mat4 m_view;
    GLfloat m_fov_user;
    GLfloat m_fov;
    GLfloat m_speed;
    GLfloat m_deltaTime;
    GLfloat m_yaw;
    GLfloat m_pitch;
    GLfloat m_mouseSensitivity;
    Direction m_direction;
    bool m_mousePress;
    bool m_moveMouse;

    static const int m_bufferSize = 5;
    float m_xoffsetBuffer[m_bufferSize], m_yoffsetBuffer[m_bufferSize];
    int m_offsetBufferEnd;

    void updateCameraVectors();
	void initOffsetBuffer();
};

#endif // CAMERA_H
