#pragma once
#include "camera.h"
#include "Model.h"

//渲染循环
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
//实例化相机，并设置其初始位置
Camera camera(glm::vec3(0.0f, 13.5f, 10.0f));
//灯位置
glm::vec3 lightPos(2.5f, 12.0f, 2.5f);
//鼠标移动
bool firstMouse = true;
float lastX = m_width / 2.0;
float lastY = m_height / 2.0;
//暂停
bool isPause = false;//是否暂停，默认没有
bool lastKeyState = false;//是否按下，默认没有
float pauseTime = 0.0f;
float pauseStartTime = 0.0f;

//窗口初始化和基础设置
GLFWwindow* Init()
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

    //每当窗口调整大小时调用回调函数
    glfwSetFramebufferSizeCallback(window,[](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
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

        camera.ProcessMouseMovement(xoffset, yoffset);
        });

    //隐藏光标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //初始化GLAD GLAD是用来管理OpenGL的函数指针的
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    stbi_set_flip_vertically_on_load(false);//翻转纹理
    glEnable(GL_DEPTH_TEST);//开启深度测试，Z-Buffer实现

    return window;
}

//检查是否按下了ESC
void processInput(GLFWwindow* window)
{
    //计算间隔时间
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    //ESC退出
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //调整相机
    float cameraSpeed = 2.5f * deltaTime;
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
unsigned int loadTexture(char const* path)
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

//查错，来自chatgpt
void error()
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

//灯随时间变化
void lampchange(GLFWwindow* window)
{
    float adjustTime = glfwGetTime() - pauseTime;//弥补暂停带来的时间偏移         
    if (!isPause) {
        //括号里面是速度，外面是半径
        glm::vec3 vec = glm::vec3(1.0f);
        vec.x = 3.5f * cos(adjustTime * 1.0f);
        vec.y = 12.5f + 4.0f * sin(adjustTime * 1.5f) / 2.0f;
        vec.z = 3.5f * sin(adjustTime * 1.0f);
        lightPos = vec;
    }
    //按下且与上一帧状态不同才会启动
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !lastKeyState) {
        isPause = !isPause;
        lastKeyState = true; // 更新状态为按下
        if (isPause) {
            // 开始暂停，记录当前的时间
            pauseStartTime = glfwGetTime();
        }
        else {
            // 结束暂停，计算暂停的时间
            pauseTime += glfwGetTime() - pauseStartTime;
        }
    }
    //直到松开按键，更新按键状态
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        lastKeyState = false;
    }
}

float vertices[] = {
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