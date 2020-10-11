#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

/*
    A model contains several meshes and textures
*/
class Model
{
public:
    Model(string path)
    {
        loadModel(path);
    }
    void Draw(const Shader& shader) const;
private:
    // model data
    vector<Mesh> meshes;
    vector<Texture> textures_loaded;
    string directory;

    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, string typeName);
};