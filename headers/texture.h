#ifndef TEXTURE_H
#define TEXTURE_H

#include "opengl.h"

#include <QImage>
#include <string>

class Texture
{
public:
	Texture();
    Texture(std::string dirPath);
    ~Texture();

	std::string name() const;
	void setName(std::string name);

    void redefineDirectory(std::string dirPath);
    bool loadFromFile(std::string filePath, GLenum target, GLenum filtering);
    void initTexture2D(void *data, GLenum format, GLenum filtering);
    void useMipMap(GLenum minFilter, GLenum magFilter);
	void useRepeat();
	void useFilteringNearest();
	void useFilteringLinear();
    GLuint getId() const;
    GLenum getTarget() const;
	void setTarget(GLenum target);
	void setFormat(GLenum format);
	void setWidth(GLsizei width);
	void setHeight(GLsizei height);
	void bind();
	GLenum getType() const;
	void setType(GLenum type);

private:
    std::string m_dirPath;
    std::string m_filePath;
	std::string m_name;
    GLuint m_id;
    GLsizei m_width;
    GLsizei m_height;
    GLenum m_formatImg;
    GLenum m_target;
	GLenum m_type;
};

#endif // TEXTURE_H

