#ifndef TEXTURE_H
#define TEXTURE_H

#include "opengl.h"

#include <QImage>
#include <string>

class Texture
{
public:
    Texture(std::string dirPath);
    ~Texture();

	std::string name() const;
	void setName(std::string name);

    void redefineDirectory(std::string dirPath);
    void loadFromFile(std::string filePath, GLenum target);
    void initTexture2D(void *data);
    void useMipMap(GLenum minFilter, GLenum magFilter);
    GLuint getId() const;
    GLenum getTarget() const;

private:
    std::string m_dirPath;
    std::string m_filePath;
	std::string m_name;
    GLuint m_id;
    GLsizei m_width;
    GLsizei m_height;
    GLenum m_format;
    GLenum m_target;
};

#endif // TEXTURE_H

