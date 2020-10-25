#include "Model.h"
#include <assimp/pbrmaterial.h>
#include <stb_image.h>


void Model::Draw(const Shader& shader) const
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::loadModel(string path)
{
	Assimp::Importer importer;
	// we don't care about the scene graph of the original model
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_PreTransformVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	// process vertex positions, normals and texture coordinates
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;
		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y= mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;
		vertex.Tangent.x = mesh->mTangents[i].x;
		vertex.Tangent.y = mesh->mTangents[i].y;
		vertex.Tangent.z = mesh->mTangents[i].z;

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> albedoMap = loadMaterialTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), albedoMap.begin(), albedoMap.end());
		vector<Texture> normalMap = loadMaterialTextures(material, aiTextureType_NORMALS);
		textures.insert(textures.end(), normalMap.begin(), normalMap.end());
		vector<Texture> ormMap = loadMaterialTextures(material, aiTextureType_UNKNOWN); //ORM maps: occlusion, roughness, metallic
		textures.insert(textures.end(), ormMap.begin(), ormMap.end());
		vector<Texture> emissionMap = loadMaterialTextures(material, aiTextureType_EMISSIVE); //ORM maps: occlusion, roughness, metallic
		textures.insert(textures.end(), emissionMap.begin(), emissionMap.end());
	}

	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
 			if (std::strcmp(textures_loaded[j].GetFilename().c_str(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			string path = directory + '/' + string(str.C_Str());
			TextureType texture_type;
			switch (type)
			{
			case aiTextureType_DIFFUSE:
				texture_type = TextureType::Albedo;
				break;
			case aiTextureType_NORMALS:
				texture_type = TextureType::Normal;
				break;
			case aiTextureType_EMISSIVE:
				texture_type = TextureType::Emissive;
				break;
			default:
				texture_type = TextureType::ORM;
				break;
			}
			textures_loaded.emplace_back(path, texture_type); // construct new texture in place
			textures.push_back(textures_loaded.back());
		}
	}
	return textures;
}
