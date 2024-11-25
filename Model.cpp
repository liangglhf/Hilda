#include "Model.h"

void Model::Draw(Shader &shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}

void Model::loadModel(std::string const &path)
{
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    /* 第二个参数为后期处理
    aiProcess_Triangulate 如果模型不是（全部）由三角形组成，则将模型所有的图元形状变换为三角形
    aiProcess_FlipUVs将在处理的时候翻转y轴的纹理坐标 */

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    //AI_SCENE_FLAGS_INCOMPLETE 场景加载不完整
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    //从path中提取信息，从0即从头开始，直到最后一个/
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // 处理节点所有的网格（如果有的话）
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // 接下来对它的子节点重复这一过程
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // 处理顶点
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector;
        // 位置
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // 法线
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        // 纹理
        if (mesh->mTextureCoords[0]) // 网格是否有纹理坐标
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    // 处理索引
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // 处理材质
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;//如果已加载过该纹理，就跳过
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            //std::strcmp 字符串比较函数，0则两者相等，<0则前者小于后者，>0则前者大于后者
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            //加载未加载过的纹理
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);//将加载的纹理push到已加载纹理中
        }
    }
    return textures;
}

//读取纹理老是出现反斜杠的路径问题，我也不知道为什么
std::string normalizePath(const std::string& path) {
    std::string normalizedPath = path;
    // 将所有反斜杠 '\' 替换为正斜杠 '/'
    std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
    // 移除路径中多余的重复正斜杠，例如 "folder//file" 变为 "folder/file"
    auto it = std::unique(normalizedPath.begin(), normalizedPath.end(),
        [](char a, char b) { return a == '/' && b == '/'; });
    normalizedPath.erase(it, normalizedPath.end());

    return normalizedPath;
}

unsigned int TextureFromFile(const char* path, const std::string& directory)
//func.h中的loadTexture()
{
    //拼接地址和文件，func.h里面没有这一步，无伤大雅
    std::string filename = normalizePath(directory + '/' + path);
    //创建对象
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        //通道
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        //加载并生成纹理
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); //第一个0是Mipmap级别
        glGenerateMipmap(GL_TEXTURE_2D);//Mipmap
        //为当前绑定的纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //释放
        stbi_image_free(data);
    }
    else
    {
        std::cerr << "Texture failed to load at path: " << path << " Reason: " << stbi_failure_reason() << std::endl;
        std::cout << "Path: " << path << std::endl;
        std::cout << "Directory: " << directory << std::endl;
        std::cout << "Full Path: " << filename << std::endl;
    }
    return textureID;
}
