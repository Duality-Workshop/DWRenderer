#ifndef MODEL_H
#define MODEL_H

#include "opengl.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>

#include "mesh.h"
#include "texture.h"
#include "material.h"

#include <vector>
#include <unordered_map>
#include <string>

class Model
{
public:
	Model(std::string path);
	~Model();

	void Draw();
	Material *material() const;

private:
	std::vector<Mesh> m_meshes;
	Material *m_material;
	std::string m_directory;
	
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	void loadMaterialTextures(aiMaterial *mat, Mesh *mesh, aiTextureType type, TextureType typeName);
	glm::mat4 aiToGLM(const aiMatrix4x4 &mat);
	glm::vec3 aiToGLM(const aiVector3D &vec);
	glm::vec3 aiToGLM(const aiColor3D &color);
	std::string aiToGLM(const aiString &string);
};

#endif // MODEL_H
