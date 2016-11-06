#ifndef SHADER_H
#define SHADER_H

#include "opengl.h" 

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

class ShaderObject
{
public:
    ShaderObject(std::string dirPath, std::string fileName, GLenum type);
    ~ShaderObject();

    void deleteShader();
    void redefineDirectory(std::string pathDirectory);
    void load();
    GLuint getId() const;
    GLuint getType() const;
    std::string &getFileName();
    int check();
    void printShaderInfoLog(GLint shader);

private:
    GLuint m_id;
    std::string m_fileName;
    std::string m_dirPath;
    GLuint m_type;

    void compile();
};

class ShaderProgram
{
public:
    ShaderProgram(std::string dirPath);
    ~ShaderProgram();

    void createProgram();
    void deleteProgram();
    void redefineDirectory(std::string pathDirectory);
    void add(ShaderObject* s);
    void createSimpleProgram(std::string vertexPath, std::string fragmentPath);
    void link();
    void reload();
    void useShaderProgram();
    GLuint getId() const;
    int check();
    void printProgramInfoLog(GLint program);
    void setUniform(const char* name, float value);
    void setUniform(const char* name, glm::vec2 value);
    void setUniform(const char* name, glm::vec3 value);
    void setUniform(const char* name, glm::vec4 value);
    void setUniform(const char* name, glm::ivec2 value);
    void setUniform(const char* name, glm::ivec3 value);
    void setUniform(const char* name, glm::ivec4 value);
    void setUniform(const char* name, glm::mat2 value);
    void setUniform(const char* name, glm::mat3 value);
    void setUniform(const char* name, glm::mat4 value);

private:
    std::string m_dirPath;
    GLuint m_id;
    GLuint vertex;
    GLuint fragment;
    std::vector<ShaderObject*> m_shaderObjects;
};

#endif // SHADER

