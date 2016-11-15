#include "model.h"

#include "texture.h"

#include <iostream>
#include <algorithm>

Model::Model(GLchar * path) {
	this->loadModel(path);
}

Model::~Model() {
}

void Model::Draw() {
	for (GLuint i = 0; i < this->m_meshes.size(); ++i)
		this->m_meshes[i].Draw();
}

void Model::loadModel(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	this->m_directory = path.substr(0, path.find_last_of('/'));

	this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
	for (GLuint i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->m_meshes.push_back(this->processMesh(mesh, scene));
	}

	for (GLuint i = 0; i < node->mNumChildren; ++i) {
		this->processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh *asMesh, const aiScene *scene) {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Mesh *mesh;

	for (GLuint i = 0; i < asMesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector;
		Mesh;
		
		// Position
		vector.x = asMesh->mVertices[i].x;
		vector.y = asMesh->mVertices[i].y;
		vector.z = asMesh->mVertices[i].z;
		vertex.position = vector;

		// Normals
		if (asMesh->HasNormals()) {
			vector.x = asMesh->mNormals[i].x;
			vector.y = asMesh->mNormals[i].y;
			vector.z = asMesh->mNormals[i].z;
			vertex.normal = vector;
		}

		// Does the mesh contain texture coordinates ? 
		if (asMesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = asMesh->mTextureCoords[0][i].x;
			vec.y = asMesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
			vertex.texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// Process indices
	for (GLuint i = 0; i < asMesh->mNumFaces; i++) {
		aiFace face = asMesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// Create the mesh
	mesh = new Mesh(vertices, indices);

	// Process material
	if (asMesh->mMaterialIndex >= 0) {
		aiMaterial *material = scene->mMaterials[asMesh->mMaterialIndex];

		//Constants
		aiString name;
		material->Get(AI_MATKEY_NAME, name);
		mesh->m_material->setName(name);

		aiColor3D color(0.f, 0.f, 0.f);
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		mesh->m_material->setAmbient(aiToGLM(color));

		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		mesh->m_material->setDiffuse(aiToGLM(color));

		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		mesh->m_material->setSpecular(aiToGLM(color));

		GLfloat shininess = 0;
		material->Get(AI_MATKEY_SHININESS, shininess);
		mesh->m_material->setShininess(shininess);

		// Textures
		loadMaterialTextures(material, mesh, aiTextureType_AMBIENT, TextureTypes::AMBIENT);
		loadMaterialTextures(material, mesh, aiTextureType_DIFFUSE, TextureTypes::DIFFUSE);
		loadMaterialTextures(material, mesh, aiTextureType_SPECULAR, TextureTypes::SPECULAR);
		loadMaterialTextures(material, mesh, aiTextureType_NORMALS, TextureTypes::NORMAL);
		
		// The Height Map can be the Normal Map
		if (!(mesh->m_material->m_texturesTypes & TextureTypes::NORMAL)) {
			loadMaterialTextures(material, mesh, aiTextureType_HEIGHT, TextureTypes::NORMAL);
		}

		// If there Diffuse Map but no Ambient Map => Ambient Map = Diffuse Map
		if ((mesh->m_material->m_texturesTypes & TextureTypes::DIFFUSE) && !(mesh->m_material->m_texturesTypes & TextureTypes::AMBIENT)) {
			loadMaterialTextures(material, mesh, aiTextureType_DIFFUSE, TextureTypes::AMBIENT);
		}
	}

	return ;
}

void Model::loadMaterialTextures(aiMaterial *mat, Mesh *mesh, aiTextureType type, TextureType typeName) {
	for (GLuint i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string name = str.C_Str();

		//Prevent loading of false textures
		if (name.size() < 4)
			return;

		auto tex = std::find(mesh->m_material->m_textures.begin(),
			mesh->m_material->m_textures.end(),
			name);
		// Texture not already loaded ?
		if (tex == mesh->m_material->m_textures.end()) {
			Texture *texture = new Texture(m_directory);
			if (texture->loadFromFile("/" + str.C_Str(),GL_TEXTURE_2D)) {
				texture->setName(name);
				// MODIFY MESH CLASS FOR PUT TEXTURES
				mesh->m_material->addTexture(typeName, name);
				mesh->m_material->updateTextureTypes(typeName);
			}
		}
	}
}

glm::mat4 Model::aiToGLM(const aiMatrix4x4 &mat) {
	glm::mat4 gmat;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			gmat[i][j] = mat[j][i];
		}
	}
	return gmat;
}

glm::vec3 Model::aiToGLM(const aiVector3D &vec) {
	return glm::vec3(vec.x, vec.y, vec.z);
}

glm::vec3 Model::aiToGLM(const aiColor3D &color) {
	return glm::vec3(color.r, color.g, color.b);
}
