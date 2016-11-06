#ifndef TEXTURE
#define TEXTURE

#define GL_GLEXT_PROTOTYPES
#ifdef _WIN32
	#include <GL/glew.h>
#endif

#include <QImage>
#include <string>

class Texture
{
public:
    Texture(std::string dirPath);
    ~Texture();

    void redefineDirectory(std::string dirPath);
    void loadFromFile(std::string filePath, GLenum target);
    void initTexture2D(void *data);
    void useMipMap(GLenum minFilter, GLenum magFilter);
    GLuint getId() const;
    GLenum getTarget() const;

private:
    std::string m_dirPath;
    std::string m_filePath;
    GLuint m_id;
    GLsizei m_width;
    GLsizei m_height;
    GLenum m_format;
    GLenum m_target;
};

#endif // TEXTURE

