#pragma once
#define STBI_WINDOWS_UTF8
#include <glad/glad.h>
#include "src/vendor/glm/glm.hpp"
#include "src/vendor/glm/gtc/matrix_transform.hpp"
#include "shader.h"
#include <stb_image.h>
#include <string>
#include <vector>
#include <algorithm>

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;  // 储存纹理的路径用于与其它纹理进行比较
};

class Mesh
{
public:
    //网格数据
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    //函数
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader &shader);
private:
    //渲染数据
    unsigned int VAO, VBO, IBO;
    //函数
    void setupMesh();
};