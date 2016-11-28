#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#define GL_GLEXT_PROTOTYPES

#include <QApplication>

#include "renderingwidget.h"

int main(int argc, char **argv)
{
    QApplication application(argc, argv);

    QGLFormat glFormat;
    glFormat.setVersion(4, 5);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    RenderingWidget widget(glFormat);
    widget.setWindowTitle("DWRenderer");
    widget.show();

    return application.exec();
}
