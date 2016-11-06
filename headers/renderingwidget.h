#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H

#define GL_GLEXT_PROTOTYPES

#include "renderer.h"

#include <QtOpenGL/QGLWidget>
#include <QTimer>

class RenderingWidget : public QGLWidget
{
    Q_OBJECT

public:
    RenderingWidget(const QGLFormat & format, QWidget * parent = 0);
    ~RenderingWidget() {}

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void keyReleaseEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void wheelEvent(QWheelEvent *e);

private:
    // Timer & FPS
    QTimer* m_timer;
    GLfloat m_fps;
    QTime *m_time;
    GLfloat m_deltaTime;
    GLfloat m_lastFrame;

    // Renderer & Camera(s)
    Camera* m_camera;
    glm::mat4 m_view;
    glm::mat4 m_projection;
    Renderer* m_renderer;

    // Movement
    GLfloat m_lastX;
    GLfloat m_lastY;
    GLfloat m_xoffset;
    GLfloat m_yoffset;
    bool m_rightClickPressed;
    bool m_mouseMoved;

    // Display
    GLuint m_screenWidth;
    GLuint m_screenHeight;
    bool m_fullscreen;
    bool m_wireframe;

public slots:
    void showFps();
};

#endif // RENDERINGWIDGET_H
