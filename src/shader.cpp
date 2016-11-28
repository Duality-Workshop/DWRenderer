#include "shader.h"

#include "material.h"
#include "model.h"

#include <iostream>
#include <fstream>
#include <algorithm>

ShaderProgram::ShaderProgram(std::string dirPath) {
    m_dirPath = dirPath;
}

ShaderProgram::~ShaderProgram() {
}

void ShaderProgram::createProgram() {
    m_id = glCreateProgram();
}

void ShaderProgram::deleteProgram() {
    glDeleteProgram(m_id);
    m_id = -1;
}

void ShaderProgram::redefineDirectory(std::string pathDirectory){
    m_dirPath = pathDirectory;
}

void ShaderProgram::add(ShaderObject* s) {
    m_shaderObjects.push_back(s);
}

void ShaderProgram::createSimpleProgram(std::string vertexPath, std::string fragmentPath) {
    ShaderObject* vertexShader = new ShaderObject(m_dirPath, vertexPath, GL_VERTEX_SHADER);
    ShaderObject* fragmentShader = new ShaderObject(m_dirPath, fragmentPath, GL_FRAGMENT_SHADER);

    this->createProgram();

    vertexShader->load();
    fragmentShader->load();

    this->add(vertexShader);
    this->add(fragmentShader);

    this->link();

    vertexShader->deleteShader();
    fragmentShader->deleteShader();
}

void ShaderProgram::link() {
    for (std::vector<ShaderObject*>::iterator itr = m_shaderObjects.begin(), stop = m_shaderObjects.end();
            itr != stop;
            ++itr) {
        glAttachShader(m_id, (*itr)->getId());
    }

    glLinkProgram(m_id);
    this->check();
}

void ShaderProgram::reload() { // Modify for the defines
    std::cout << "Reload shaders..." << std::endl;
    bool ok = true;

    for (unsigned int i = 0; ok && i < m_shaderObjects.size(); ++i) {
        glDetachShader(m_id, m_shaderObjects[i]->getId());
        m_shaderObjects[i]->deleteShader();
		m_shaderObjects[i]->resetShaderStream();
        std::cout << m_shaderObjects[i]->getFileName() << std::endl;
		if (m_shaderObjects[i]->getModelPresent()) {
			m_shaderObjects[i]->load(m_shaderObjects[i]->getModel());
		} else 
			m_shaderObjects[i]->load();
        ok |= m_shaderObjects[i]->check();
    }

    if (ok) {
        this->deleteProgram();
        this->createProgram();
        this->link();
        this->check();
    }
}

GLuint ShaderProgram::getId() const {
    return m_id;
}

void ShaderProgram::useShaderProgram() {
    glUseProgram(m_id);
}

int ShaderProgram::check() {
    GLint success;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED:\n" << std::endl;
        for (unsigned int i = 0; i < m_shaderObjects.size(); ++i) {
            std::cerr << m_shaderObjects[i]->getFileName() << std::endl;
        }
        printProgramInfoLog(m_id);
        return 0;
    }

    return 1;
}

void ShaderProgram::printProgramInfoLog(GLint program) {
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        glGetProgramInfoLog(program, infoLogLen, &charsWritten, infoLog);
        std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
        delete [] infoLog;
    }
}

void ShaderProgram::setUniform(const char *name, float value) {
    glUniform1f(glGetUniformLocation(m_id, name), value);
}

void ShaderProgram::setUniform(const char *name, glm::vec2 value) {
    glUniform2fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const char *name, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const char *name, glm::vec4 value) {
    glUniform4fv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const char *name, glm::ivec2 value) {
    glUniform2iv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const char *name, glm::ivec3 value) {
    glUniform3iv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const char *name, glm::ivec4 value) {
    glUniform4iv(glGetUniformLocation(m_id, name), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const char *name, glm::mat2 value) {
    glUniformMatrix2fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const char *name, glm::mat3 value) {
    glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setUniform(const char *name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(value));
}

/*********************
 * ShaderObject
*********************/

ShaderObject::ShaderObject(std::string dirPath, std::string fileName, GLenum type) {
    m_fileName = fileName;
    m_dirPath = dirPath;
    m_type = type;
	m_model_Present = false;
}

ShaderObject::~ShaderObject() {
}

std::string ShaderObject::setDefines(Model *model) {
	std::string shaderDefine = "";
	std::vector<TextureType> texturesType;

	// Get version GLSL
	std::string v;
	const GLubyte *versionCore = glGetString(GL_SHADING_LANGUAGE_VERSION);
	for (int i = 0; i < 4; ++i)
		if (versionCore[i] != '.')
			v += versionCore[i];

	shaderDefine += "#version " + v + " core\n\n";
	
	texturesType = model->material()->keysTextures();
	
	if(model->material()->texturesName().size() > 0) {
		if (std::find(texturesType.begin(), texturesType.end(), AMBIENT) != texturesType.end())
			shaderDefine += "#define AMBIENT_MAP\n";
		if (std::find(texturesType.begin(), texturesType.end(), DIFFUSE) != texturesType.end())
			shaderDefine += "#define DIFFUSE_MAP\n";
		if (std::find(texturesType.begin(), texturesType.end(), SPECULAR) != texturesType.end())
			shaderDefine += "#define SPECULAR_MAP\n";
		if (std::find(texturesType.begin(), texturesType.end(), NORMAL) != texturesType.end())
			shaderDefine += "#define NORMAL_MAP\n";
	}

	return shaderDefine;
}

void ShaderObject::deleteShader() {
    glDeleteShader(m_id);
    m_id = -1;
}

void ShaderObject::redefineDirectory(std::string pathDirectory) {
    m_dirPath = pathDirectory;
}

void ShaderObject::load() {
    std::string fileName;
    std::string shaderCode;
    std::ifstream shaderFile;
    const GLchar* sShaderCode = "";

    fileName = m_dirPath + m_fileName;

    shaderFile.exceptions(std::ifstream::badbit);
    try {
        shaderFile.open(fileName);
        m_shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        shaderCode = m_shaderStream.str();
    }
    catch(std::ifstream::failure e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULY_READ" << std::endl;
    }

    sShaderCode = shaderCode.c_str();

    m_id = glCreateShader(m_type);
    glShaderSource(m_id, 1, &sShaderCode, NULL);

    this->compile();
}

void ShaderObject::load(Model *model) {
	if (!this->m_model_Present) {
		m_model = model;
		m_model_Present = true;
	}
	m_shaderStream << setDefines(model).c_str();
	m_shaderStream << "\n";
	this->load();
}

void ShaderObject::compile() {
    glCompileShader(m_id);
    this->check();
}

int ShaderObject::check() {
    GLint success;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED:\n" << m_fileName << std::endl;
        printShaderInfoLog(m_id);
        return 0;
    }
    return 1;

}

GLuint ShaderObject::getId() const {
    return m_id;
}

GLuint ShaderObject::getType() const {
    return m_type;
}

std::string &ShaderObject::getFileName() {
    return m_fileName;
}

Model *ShaderObject::getModel() {
	return m_model;
}

bool ShaderObject::getModelPresent() {
	return m_model_Present;
}

void ShaderObject::resetShaderStream() {
	m_shaderStream.str("");
	m_shaderStream.clear();
}

void ShaderObject::printShaderInfoLog(GLint shader) {
    int infoLogLen = 0;
    int charsWritten = 0;
    GLchar *infoLog;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
    if (infoLogLen > 0) {
        infoLog = new GLchar[infoLogLen];
        glGetShaderInfoLog(shader, infoLogLen, &charsWritten, infoLog);
        std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
        delete [] infoLog;
    }

    int sourceLenght;
    glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &sourceLenght);
    infoLog = new GLchar[sourceLenght];
    glGetShaderSource(shader, sourceLenght, &charsWritten, infoLog);
    std::cerr << "InfoLog:" << std::endl << infoLog << std::endl;
    delete [] infoLog;
}
