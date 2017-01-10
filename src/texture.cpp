#include "texture.h"

#include <QGLWidget>
#include <iostream>

Texture::Texture() {
}

Texture::Texture(std::string dirPath) {
    m_dirPath = dirPath;
}

Texture::~Texture() {
}

std::string Texture::name() const {
	return m_name;
}

void Texture::setName(std::string name) {
	m_name = name;
}

void Texture::redefineDirectory(std::string dirPath){
    m_dirPath = dirPath;
}

bool Texture::loadFromFile(std::string filePath, GLenum target, GLenum filtering) {
    QImage img;
    QImage glImg;

    m_filePath = m_dirPath + filePath;

    if(!img.load(m_filePath.c_str())) {
        std::cerr << "ERROR::LOADING::IMAGE:" << m_filePath << std::endl;
		return false;
    }

    glImg = QGLWidget::convertToGLFormat(img);
    if(glImg.isNull()) {
        std::cerr << "ERROR::CONVERSION::TO::GL::FORMAT" << std::endl;
		return false;
    }

    m_width = glImg.width();
    m_height = glImg.height();
	m_type = GL_UNSIGNED_BYTE;

    m_formatImg = GL_RGB;
    if(glImg.hasAlphaChannel()) {
        m_formatImg = GL_RGBA;
    }

    m_target = target;
    glBindTexture(m_target, 0);

    if(target == GL_TEXTURE_2D)
        this->initTexture2D(glImg.bits(), m_formatImg, filtering);
    /* Other type of target => Later */
	
	return true;
}

void Texture::initTexture2D(void *data, GLenum format, GLenum filtering) {
    glGenTextures(1, &m_id);
    glBindTexture(m_target, m_id);
    glTexImage2D(m_target, 0, format, m_width, m_height, 0, m_formatImg, m_type, data);
	if (filtering == GL_LINEAR)
		this->useFilteringLinear();
	if (filtering == GL_NEAREST)
		this->useFilteringNearest();
    glBindTexture(m_target, 0);
}

void Texture::useMipMap(GLenum minFilter, GLenum magFilter) {
    glBindTexture(m_target, m_id);
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, magFilter);
    glGenerateMipmap(m_target);
	glBindTexture(m_target, 0);
}

void Texture::useRepeat() {
	glBindTexture(m_target, m_id);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glBindTexture(m_target, 0);
}

void Texture::useFilteringNearest() {
	glBindTexture(m_target, m_id);
	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(m_target, 0);
}

void Texture::useFilteringLinear() {
	glBindTexture(m_target, m_id);
	glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLuint Texture::getId() const {
    return m_id;
}

GLenum Texture::getTarget() const {
    return m_target;
}

void Texture::setTarget(GLenum target) {
	m_target = target;
}

void Texture::setFormat(GLenum format) {
	m_formatImg = format;
}

void Texture::setWidth(GLsizei width) {
	m_width = width;
}

void Texture::setHeight(GLsizei height) {
	m_height = height;
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, m_id);
}

GLenum Texture::getType() const {
	return m_type;
}

void Texture::setType(GLenum type) {
	m_type = type;
}
