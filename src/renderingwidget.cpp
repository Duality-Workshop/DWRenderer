#include "renderingwidget.h"

#include <QCoreApplication>
#include <QKeyEvent>
#include <iostream>
#include <fstream>
#include <string>

RenderingWidget::RenderingWidget(const QGLFormat & format, QWidget * parent)
    : QGLWidget(format, parent) {

    // GameLoop all the time
    m_timer = new QTimer(this);
    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    m_timer->start(16);

    // FPS
    m_fps = 0;
    QTimer* fpsTimer = new QTimer();
    QObject::connect(fpsTimer, SIGNAL(timeout()), this, SLOT(showFps()));
    fpsTimer->start(1000);

    // System
    m_screenWidth = 800;
    m_screenHeight = 600;
    m_fullscreen = false;

    // Events for movement
    m_time = new QTime();
    m_mouseMoved = false;
    m_rightClickPressed = false;
    m_lastX = 0.0f;
    m_lastY = 0.0f;
}

void RenderingWidget::initializeGL() {
    m_renderer = std::make_unique<MainRenderer>(m_screenWidth,m_screenHeight);
    m_renderer->initializeRenderer();

    // Timer for framerate
    m_deltaTime = 0.0f;
    m_lastFrame = 0.0f;
    m_time->start();
}

void RenderingWidget::resizeGL(int w, int h) {
    m_screenWidth = w;
    m_screenHeight = h;
	// Resize
    m_renderer->setWidth(w);
    m_renderer->setHeight(h);
    m_renderer->initializeRenderer();
	m_renderer->getCamera()->initCamera();
    this->updateGL();
}

void RenderingWidget::paintGL() {
    // Time
    GLfloat currentFrame = m_time->elapsed()/1000.0f;
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;
    // Movement
    if (m_mouseMoved)
		m_renderer->getCamera()->processMouseMovement(m_xoffset, m_yoffset);
    else
		m_renderer->getCamera()->processMouseMovement(0, 0);
	m_renderer->getCamera()->updateCamera(m_deltaTime);

    // Rendering
    m_renderer->draw();
    m_mouseMoved = false;
    ++m_fps;
}

void RenderingWidget::keyPressEvent(QKeyEvent *e) {

    switch (e->key())
    {
        case Qt::Key_F1:
            if(!m_fullscreen){
                this->setWindowState(Qt::WindowFullScreen);
                m_fullscreen = true;
            } else {
                m_fullscreen = false;
                this->setWindowState(Qt::WindowMaximized);
            }
        break;

        case Qt::Key_Escape:
            m_renderer->freeMemory();
            QCoreApplication::instance()->quit();
        break;

        case Qt::Key_W:
			if (!m_renderer->getWireframe()) {
				std::cout << "Wireframe on" << std::endl;
				m_renderer->setWireframe(true);
			}
			else {
				std::cout << "Wireframe off" << std::endl;
				m_renderer->setWireframe(false);
			}
        break;

		case Qt::Key_H:
			if (!m_renderer->getHdr()) {
				std::cout << "HDR on" << std::endl;
				m_renderer->setHdr(true);
			}
			else {
				std::cout << "HDR off" << std::endl;
				m_renderer->setHdr(false);
			}
			m_renderer->reload();
		break;

		case Qt::Key_B:
			if (!m_renderer->bloom()) {
				std::cout << "Bloom on" << std::endl;
				m_renderer->setBloom(true);
			}
			else {
				std::cout << "Bloom off" << std::endl;
				m_renderer->setBloom(false);
			}
			m_renderer->reload();
		break;

        case Qt::Key_C:
            m_renderer->getCamera()->initCamera();
        break;

        case Qt::Key_R: // Reload shaders with Ctrl+R
            if(e->modifiers() == Qt::ControlModifier)
                m_renderer->reload();
            else if(e->modifiers() == Qt::ShiftModifier) // Pitch camera
                m_renderer->getCamera()->processKeyPress(Camera::KEY_PITCHUP);
            else{ // Up camera
                m_renderer->getCamera()->processKeyPress(Camera::KEY_UP);
            }
        break;

        // Camera
        case Qt::Key_Z:
            m_renderer->getCamera()->processKeyPress(Camera::KEY_FORWARD);
        break;

        case Qt::Key_Q:
            m_renderer->getCamera()->processKeyPress(Camera::KEY_LEFT);
        break;

        case Qt::Key_S:
            m_renderer->getCamera()->processKeyPress(Camera::KEY_BACKWARD);
        break;

        case Qt::Key_D:
            m_renderer->getCamera()->processKeyPress(Camera::KEY_RIGHT);
        break;

        case Qt::Key_F:
            if(e->modifiers() == Qt::ShiftModifier)
                m_renderer->getCamera()->processKeyPress(Camera::KEY_PITCHDOWN);
            else
                m_renderer->getCamera()->processKeyPress(Camera::KEY_DOWN);
        break;

        case Qt::Key_T:
            if(e->modifiers() == Qt::ShiftModifier)
                m_renderer->getCamera()->resetYaw();
            else
                m_renderer->getCamera()->resetPitch();
        break;

        case Qt::Key_A:
            m_renderer->getCamera()->processKeyPress(Camera::KEY_YAWLEFT);
        break;

        case Qt::Key_E:
            m_renderer->getCamera()->processKeyPress(Camera::KEY_YAWRIGHT);
        break;

        default:
            QGLWidget::keyPressEvent(e);
    }

    this->updateGL();
}

void RenderingWidget::keyReleaseEvent(QKeyEvent *e) {
    switch(e->key()){
        // Camera
        case Qt::Key_Z:
            m_renderer->getCamera()->processKeyRelease(Camera::KEY_FORWARD);
        break;

        case Qt::Key_Q:
            m_renderer->getCamera()->processKeyRelease(Camera::KEY_LEFT);
        break;

        case Qt::Key_S:
            m_renderer->getCamera()->processKeyRelease(Camera::KEY_BACKWARD);
        break;

        case Qt::Key_D:
            m_renderer->getCamera()->processKeyRelease(Camera::KEY_RIGHT);
        break;

        case Qt::Key_R:
            if(e->modifiers() && Qt::ShiftModifier)
                m_renderer->getCamera()->processKeyRelease(Camera::KEY_PITCHUP);
            else
                m_renderer->getCamera()->processKeyRelease(Camera::KEY_UP);
        break;

        case Qt::Key_F:
            if(e->modifiers() && Qt::ShiftModifier)
                m_renderer->getCamera()->processKeyRelease(Camera::KEY_PITCHDOWN);
            else
                m_renderer->getCamera()->processKeyRelease(Camera::KEY_DOWN);
        break;

        case Qt::Key_A:
            m_renderer->getCamera()->processKeyRelease(Camera::KEY_YAWLEFT);
        break;

        case Qt::Key_E:
            m_renderer->getCamera()->processKeyRelease(Camera::KEY_YAWRIGHT);
        break;

        default:
            QGLWidget::keyReleaseEvent(e);
    }
}

void RenderingWidget::mousePressEvent(QMouseEvent *e) {
    switch(e->button()){
        case Qt::RightButton:
            m_rightClickPressed = true;
            m_lastX = e->x();
            m_lastY = e->y();
            m_renderer->getCamera()->beginMouseMove();
        break;

        default:
            QGLWidget::mousePressEvent(e);
    }
}

void RenderingWidget::mouseReleaseEvent(QMouseEvent *e) {
    switch(e->button()){
        case Qt::RightButton:
            m_rightClickPressed = false;
            m_renderer->getCamera()->endMouseMove();
        break;

        default:
            QGLWidget::mousePressEvent(e);
    }
}

void RenderingWidget::mouseMoveEvent(QMouseEvent *e) {
    if(m_rightClickPressed){
        m_mouseMoved = true;
        m_xoffset = e->x() - m_lastX;
        m_yoffset = m_lastY - e->y();
        m_lastX = e->x();
        m_lastY = e->y();
        this->updateGL();
    }
}

void RenderingWidget::wheelEvent(QWheelEvent *e) {
    m_renderer->getCamera()->processMouseScroll(e->delta()/60.0f);
    this->updateGL();
}

void RenderingWidget::showFps() {
    //std::cout << m_fps << std::endl;
    m_fps = 0.0f;
}
