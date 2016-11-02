#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat fov, GLfloat yaw, GLfloat pitch) {
    m_position_init = position;
    m_position = position;
    m_worldUp = up;
    m_fov_user = fov;
    m_fov = fov;
    m_yaw = yaw;
    m_pitch = pitch;
    m_direction = NONE;
    m_mousePress = false;
    m_moveMouse = false;
    this->updateCameraVectors();
	this->initOffsetBuffer();
}

Camera::~Camera() {
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_target, m_worldUp);;
}

GLfloat Camera::getFov() const {
    return m_fov;
}

glm::vec3 Camera::getPosition() const {
    return m_position;
}

void Camera::setSpeed(const GLfloat speed) {
    m_speed = speed;
}

void Camera::setMouseSensitivity(const GLfloat mouseSensitivity) {
    m_mouseSensitivity = mouseSensitivity;
}

void Camera::resetPitch() {
    m_pitch = 0.0f;
    this->updateCameraVectors();
}

void Camera::resetYaw() {
    m_yaw = -90.0f;
    this->updateCameraVectors();
}

void Camera::initCamera() {
    m_position = m_position_init;
    m_target = glm::vec3(0.0f,0.0f,-1.0f);
    m_worldUp = glm::vec3(0.0f,1.0f,0.0f);
    m_fov = m_fov_user;
    m_pitch = 0.0f;
    m_yaw = -90.f;
    m_direction = NONE;
    m_mousePress = false;
    m_moveMouse = false;
    this->updateCameraVectors();
}

void Camera::updateCamera(GLfloat deltaTime) {
    GLfloat velocity = m_speed * deltaTime;

    switch(m_direction) {
        case FORWARD:
            m_position += velocity * m_target;
        break;
        case BACKWARD:
            m_position -= velocity * m_target;
        break;
        case RIGHT:
            m_position += velocity * m_right;
        break;
        case LEFT:
            m_position -= velocity * m_right;
        break;
        case UP:
            m_position += velocity * m_worldUp;
        break;
        case DOWN:
            m_position -= velocity * m_worldUp;
        break;
        case FORWARDLEFT:
            m_position += (velocity * m_target - velocity * m_right);
        break;
        case FORWARDRIGHT:
            m_position += (velocity * m_target + velocity * m_right);
        break;
        case BACKWARDLEFT:
            m_position -= (velocity * m_target + velocity * m_right);
        break;
        case BACKWARDRIGHT:
            m_position += (-velocity * m_target + velocity * m_right);
        break;
        case YAWLEFT:
            m_yaw -= 10.0 * velocity;
            this->updateCameraVectors();
        break;
        case YAWRIGHT:
            m_yaw += 10.0 * velocity;
            this->updateCameraVectors();
        break;
        case PITCHUP:
            m_pitch += 6.0f * velocity;
            if (this->m_pitch > 89.0f)
                this->m_pitch = 89.0f;
            this->updateCameraVectors();
        break;
        case PITCHDOWN:
            m_pitch -= 6.0f * velocity;
            if (this->m_pitch < -89.0f)
                this->m_pitch = -89.0f;
            this->updateCameraVectors();
        break;
        default:
            m_position = m_position;
    }

    float xoff = 0;
    float yoff = 0;
    for (int i = 0; i < m_bufferSize; i++) {
        xoff += m_xoffsetBuffer[i];
        yoff += m_yoffsetBuffer[i];
    }
    xoff *= 0.1;
    yoff *= 0.1;

    if (m_moveMouse) {
        this->m_yaw   += xoff;
        this->m_pitch += yoff;

        if (this->m_pitch > 89.0f)
            this->m_pitch = 89.0f;
        if (this->m_pitch < -89.0f)
            this->m_pitch = -89.0f;

        this->updateCameraVectors();
    }
}

void Camera::processKeyPress(Key key) {
    switch (key) {
        case KEY_FORWARD:
            if (m_direction == LEFT)
                m_direction = FORWARDLEFT;
            else if (m_direction == RIGHT)
                m_direction = FORWARDRIGHT;
            else
                m_direction = FORWARD;
        break;
        case KEY_BACKWARD:
            if (m_direction == LEFT)
                m_direction = BACKWARDLEFT;
            else if (m_direction == RIGHT)
                m_direction = BACKWARDRIGHT;
            else
                m_direction = BACKWARD;
        break;
        case KEY_RIGHT:
            if (m_direction == FORWARD)
                m_direction = FORWARDRIGHT;
            else if (m_direction == BACKWARD)
                m_direction = BACKWARDRIGHT;
            else
                m_direction = RIGHT;
        break;
        case KEY_LEFT:
            if (m_direction == FORWARD)
                m_direction = FORWARDLEFT;
            else if (m_direction == BACKWARD)
                m_direction = BACKWARDLEFT;
            else
                m_direction = LEFT;
        break;
        case KEY_UP:
            m_direction = UP;
        break;
        case KEY_DOWN:
            m_direction = DOWN;
        break;
        case KEY_YAWLEFT:
            m_direction = YAWLEFT;
        break;
        case KEY_YAWRIGHT:
            m_direction = YAWRIGHT;
        break;
        case KEY_PITCHUP:
            m_direction = PITCHUP;
        break;
        case KEY_PITCHDOWN:
            m_direction = PITCHDOWN;
        break;
    }
}

void Camera::processKeyRelease(Key key)
{
    switch (key) {
        case KEY_FORWARD:
            if (m_direction == Camera::FORWARD)
                m_direction = Camera::NONE;
            else if (m_direction == Camera::FORWARDLEFT)
                m_direction = Camera::LEFT;
            else if (m_direction == Camera::FORWARDRIGHT)
                m_direction = Camera::RIGHT;
        break;
        case KEY_BACKWARD:
            if (m_direction == Camera::BACKWARD)
                m_direction = Camera::NONE;
            else if (m_direction == Camera::BACKWARDLEFT)
                m_direction = Camera::LEFT;
            else if (m_direction == Camera::BACKWARDRIGHT)
                m_direction = Camera::RIGHT;
        break;
        case KEY_RIGHT:
            if (m_direction == Camera::RIGHT)
                m_direction = Camera::NONE;
            else if (m_direction == Camera::FORWARDRIGHT)
                m_direction = Camera::FORWARD;
            else if (m_direction == Camera::BACKWARDRIGHT)
                m_direction = Camera::BACKWARD;
        break;
        case KEY_LEFT:
            if (m_direction == Camera::LEFT)
                m_direction = Camera::NONE;
            else if (m_direction == Camera::FORWARDLEFT)
                m_direction = Camera::FORWARD;
            else if (m_direction == Camera::BACKWARDLEFT)
                m_direction = Camera::BACKWARD;
        break;
        case KEY_UP:
            if (m_direction == Camera::UP)
                m_direction = Camera::NONE;
        break;
        case KEY_DOWN:
            if (m_direction == Camera::DOWN)
                m_direction = Camera::NONE;
        break;
        case KEY_YAWLEFT:
            if (m_direction == Camera::YAWLEFT)
                m_direction = Camera::NONE;
        break;
        case KEY_YAWRIGHT:
            if (m_direction == Camera::YAWRIGHT)
                m_direction = Camera::NONE;
        break;
        case KEY_PITCHUP:
            if (m_direction == Camera::PITCHUP)
                m_direction = Camera::NONE;
        break;
        case KEY_PITCHDOWN:
            if (m_direction == Camera::PITCHDOWN)
                m_direction = Camera::NONE;
        break;
    }
}

void Camera::beginMouseMove() {
    initOffsetBuffer();
    m_moveMouse = true;
}

void Camera::endMouseMove() {
    m_moveMouse = false;
}

void Camera::initOffsetBuffer() {
    for (int i = 0; i < m_bufferSize; i++) {
        m_xoffsetBuffer[i] = 0;
        m_yoffsetBuffer[i] = 0;
    }
    m_offsetBufferEnd = 0;
}

void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset)
{
    m_xoffsetBuffer[m_offsetBufferEnd] = xoffset * this->m_mouseSensitivity;
    m_yoffsetBuffer[m_offsetBufferEnd] = yoffset * this->m_mouseSensitivity;

    ++m_offsetBufferEnd;
    if (m_offsetBufferEnd == m_bufferSize)
        m_offsetBufferEnd = 0;

}

void Camera::processMouseScroll(GLfloat yoffset) {
    if (m_fov >= 1.0f && m_fov <= m_fov_user)
        m_fov -= yoffset;
    if (m_fov <= 1.0f)
        m_fov = 1.0f;
    if (m_fov >= m_fov_user)
        m_fov = m_fov_user;
}

void Camera::updateCameraVectors() {
    glm::vec3 target;
    target.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    target.y = sin(glm::radians(m_pitch));
    target.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_target = glm::normalize(target);
    m_right = glm::normalize(glm::cross(m_target, m_worldUp));
    m_up = glm::cross(m_right, m_target);
}
