#include "texture.h"

#include <QGLWidget>
#include <iostream>

Texture::Texture(std::string dirPath) {
    m_dirPath = dirPath;
}

Texture::~Texture() {
}

void Texture::redefineDirectory(std::string dirPath){
    m_dirPath = dirPath;
}

void Texture::loadFromFile(std::string filePath, GLenum target) {
    QImage img;
    QImage glImg;

    m_filePath = m_dirPath + filePath;

    if(!img.load(m_filePath.c_str())) {
        std::cerr << "ERROR::LOADING::IMAGE:" << m_filePath << std::endl;
    }

    glImg = QGLWidget::convertToGLFormat(img);
    if(glImg.isNull()) {
        std::cerr << "ERROR::CONVERSION::TO::GL::FORMAT" << std::endl;
    }

    m_width = glImg.width();
    m_height = glImg.height();

    m_format = GL_RGB;
    if(glImg.hasAlphaChannel()) {
        m_format = GL_RGBA;
    }

    m_target = target;
    glBindTexture(m_target, 0);

    if(target == GL_TEXTURE_2D)
        this->initTexture2D(glImg.bits());
    /* Other type of target => Later */
}

void Texture::initTexture2D(void* data) {
    glGenTextures(1, &m_id);
    glBindTexture(m_target, m_id);
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(m_target, 0, m_format, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, data);
    glBindTexture(m_target, 0);
}

void Texture::useMipMap(GLenum minFilter, GLenum magFilter) {
    glBindTexture(m_target, m_id);
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, magFilter);
    glGenerateMipmap(m_target);
}

GLuint Texture::getId() const {
    return m_id;
}

GLenum Texture::getTarget() const {
    return m_target;
}
