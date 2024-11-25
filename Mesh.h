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
    std::string path;  // ���������·������������������бȽ�
};

class Mesh
{
public:
    //��������
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    //����
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader &shader);
private:
    //��Ⱦ����
    unsigned int VAO, VBO, IBO;
    //����
    void setupMesh();
};