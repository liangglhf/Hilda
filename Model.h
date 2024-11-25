#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

unsigned int TextureFromFile(const char* path, const std::string& directory);

class Model
{
public:
    Model(const char* path)
    {
        loadModel(path);
    }
    void Draw(Shader &shader);
private:
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;//已加载过的纹理
    std::string directory;
    void loadModel(std::string const &path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);//辅助函数
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};