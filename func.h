#pragma once
#define M_PI 3.14159265358979323846
#include <cmath> 
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Model.h"
#include "camera.h"

//渲染循环
inline float deltaTime = 0.0f; // 当前帧与上一帧的时间差
inline float lastFrame = 0.0f; // 上一帧的时间
//鼠标移动
inline bool firstMouse = true;
inline float lastX = m_width / 2.0;
inline float lastY = m_height / 2.0;
//暂停
inline bool isPause = false; //是否暂停，默认没有
inline bool spaceLast = false; //是否按下，默认没有
inline float pauseTime = 0.0f;
inline float pauseStartTime = 0.0f;
//视角光
inline bool spotLight = true;//默认视角光开
inline bool spotlightLast = false;//是否按下，默认没有
//alt隐藏光标
inline bool mousehide = true; //默认光标隐藏
//开关
inline bool Furina = true;//默认黑Furina
inline bool lockLight = false;//默认没有锁定灯
inline bool grass = true;//默认有草
inline bool windowPNG = true;//默认有窗户
inline bool plane = true;//默认有地板
//后期处理
inline bool issharpen = false;
inline bool isedge = false;
inline bool isemboss = false;
inline bool isembossInvert = false;
inline bool issolarize = false;
inline bool isblur = false;
//灯位置
inline glm::vec3 lightSpeed(1.0f, 1.5f, 1.0f);
inline glm::vec3 lightPos(1.0f);
//球位置
inline glm::vec3 ballSpeed(0.25f, 0.0f, 0.25f);
inline glm::vec3 ballPos(1.0f);
//球折射率
inline const char* materials[] = {//imgui规定必须const char*
    "Glass", "Vacuum", "Air", "Water", "Ice", "Alcohol", "Quartz", "Acrylic", "Diamond", "Sapphire"
};
inline float IOR[] = {
    1.52f, 1.0f, 1.0003f, 1.333f, 1.31f, 1.36f, 1.46f, 1.49f, 2.42f, 1.77f
};
inline int curMaterial = 5; //默认为玻璃
inline float ratio = 1.0f / IOR[curMaterial];
//实例化相机，并设置其初始位置
inline Camera camera(glm::vec3(0.0f, 15.0f, 25.0f));
//地板高度
inline float planeHeight = 5.0f;


//窗口初始化和基础设置
inline GLFWwindow* Init()
{
    //初始化 实例化窗口
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //创建窗口
    GLFWwindow* window = glfwCreateWindow(m_width, m_height, "Hilda", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);//失败时直接退出程序
    }

    //把“工作空间”绑定到指定窗口，即把窗口的上下文设置为当前线程的主上下文
    glfwMakeContextCurrent(window);
    //隐藏光标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //每当窗口调整大小时调用回调函数
    glfwSetFramebufferSizeCallback(window,[](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        });

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS) {
                if (mousehide) {
                    // 按下 ALT 键时，显示光标                 
                    mousehide = false;
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    glfwSetCursorPos(window, m_width / 2.0, m_height / 2.0);
                }
                else {
                    // 按下 ALT 键时，隐藏光标
                    mousehide = true;
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                }
            }
        });

    //每当鼠标滚轮时调用回调函数
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
        });

    //每当移动鼠标时调用回调函数
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xposIn, double yposIn) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        //避免下一步xoffset和yoffset会为0，导致进入窗口后会跳一下
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        //计算每帧鼠标移动距离并更新
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        if (mousehide)
            camera.ProcessMouseMovement(xoffset, yoffset);
        });

    //初始化GLAD GLAD是用来管理OpenGL的函数指针的
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    stbi_set_flip_vertically_on_load(false);//翻转纹理
    
    return window;
}

//检查是否按下了ESC
inline void processInput(GLFWwindow* window)
{
    //计算间隔时间
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    //ESC退出
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //调整相机
    float cameraSpeed = 4.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);  
}

//加载纹理
inline unsigned int loadTexture(char const* path)
{
    //创建对象
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //释放
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}
//加载纹理，天空盒
inline unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

//查错，来自chatgpt
inline void error()
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        switch (err) {
        case GL_INVALID_OPERATION:
            std::cout << "GL_INVALID_OPERATION: an unacceptable value is specified for an enumerated argument." << std::endl;
            break;
        case GL_INVALID_VALUE:
            std::cout << "GL_INVALID_VALUE: a numeric argument is out of range." << std::endl;
            break;
        case GL_INVALID_ENUM:
            std::cout << "GL_INVALID_ENUM: an unacceptable value is specified for an enumerated argument." << std::endl;
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION: the framebuffer object is not complete." << std::endl;
            break;
        case GL_OUT_OF_MEMORY:
            std::cout << "GL_OUT_OF_MEMORY: there is not enough memory left to execute the command." << std::endl;
            break;
        default:
            std::cout << "Unknown OpenGL error: " << err << std::endl;
        }
    }
}

inline float cubeVertices[] = {
    //位置
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

inline float planeVertices[] = {
    // positions          // texture Coords 
     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

     5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
     5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

inline float gwVertices[] = {//gw即grass和window
    // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

    0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
    1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
    1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

inline std::vector<glm::vec3> gw
{
    glm::vec3(-1.5f, 0.0f, -0.48f),
    glm::vec3(1.5f, 0.0f, 0.51f),
    glm::vec3(0.0f, 0.0f, 0.7f),
    glm::vec3(-0.3f, 0.0f, -2.3f),
    glm::vec3(0.5f, 0.0f, -0.6f)
};

inline float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};

//正常
inline float kernel[9] = {
    0, 0, 0,
    0, 1, 0,
    0, 0, 0
};
//锐化
inline float sharpen[9] = {
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
};
//边缘检测
inline float edge[9] = {
    1, 1, 1,
    1,-8, 1,
    1, 1, 1
};
//浮雕
inline float emboss[9] = {
    -2,-1, 0,
    -1, 1, 1,
     0, 1, 2
};
//反转浮雕
inline float embossInvert[9] = {
     2,  1,  0,
     1, -1, -1,
     0, -1, -2
};
//太阳化
inline float solarize[9] = {
    0, 1, 0,
    1, -4, 1,
    0, 1, 0
};
//高斯模糊
inline float blur[9] = {
    1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0,
    2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
    1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0
};

//天空盒
inline std::vector<std::string> faces
{
    "res/Texture/containers/skybox/right.jpg",
    "res/Texture/containers/skybox/left.jpg",
    "res/Texture/containers/skybox/top.jpg",
    "res/Texture/containers/skybox/bottom.jpg",
    "res/Texture/containers/skybox/front.jpg",
    "res/Texture/containers/skybox/back.jpg"
};
inline float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

//生成球
inline std::vector<float> generateBall() {
    std::vector<float> ballVertices;
    unsigned int stackCount = 18;
    unsigned int sectorCount = 36;
    float radius = 1.0f;
    for (unsigned int i = 0; i < stackCount; ++i) {
        for (unsigned int j = 0; j < sectorCount; ++j) {
            // 获取四个顶点（当前层和下一层）
            auto getVertex = [&](int stack, int sector) {
                float theta = M_PI * stack / stackCount;
                float phi = 2 * M_PI * sector / sectorCount;
                float x = sin(theta) * cos(phi);
                float y = cos(theta);
                float z = sin(theta) * sin(phi);
                return glm::vec3(x, y, z) * radius;
                };

            glm::vec3 v1 = getVertex(i, j);
            glm::vec3 v2 = getVertex(i + 1, j);
            glm::vec3 v3 = getVertex(i, j + 1);
            glm::vec3 v4 = getVertex(i + 1, j + 1);

            // 添加两个三角形（v1-v2-v3 和 v2-v4-v3）
            for (const auto& v : { v1, v2, v3, v2, v4, v3 }) {
                 ballVertices.push_back(v.x);
                 ballVertices.push_back(v.y);
                 ballVertices.push_back(v.z);

                 glm::vec3 normal = glm::normalize(v);
                 ballVertices.push_back(normal.x);
                 ballVertices.push_back(normal.y);
                 ballVertices.push_back(normal.z);
            }
        }
    }
    return ballVertices;
}