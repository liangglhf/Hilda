#pragma once
#include "camera.h"
#include "Model.h"

//��Ⱦѭ��
float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��
//ʵ������������������ʼλ��
Camera camera(glm::vec3(0.0f, 13.5f, 10.0f));
//��λ��
glm::vec3 lightPos(2.5f, 12.0f, 2.5f);
//����ƶ�
bool firstMouse = true;
float lastX = m_width / 2.0;
float lastY = m_height / 2.0;
//��ͣ
bool isPause = false;//�Ƿ���ͣ��Ĭ��û��
bool lastKeyState = false;//�Ƿ��£�Ĭ��û��
float pauseTime = 0.0f;
float pauseStartTime = 0.0f;

//���ڳ�ʼ���ͻ�������
GLFWwindow* Init()
{
    //��ʼ�� ʵ��������
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //��������
    GLFWwindow* window = glfwCreateWindow(m_width, m_height, "Hilda", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);//ʧ��ʱֱ���˳�����
    }

    //�ѡ������ռ䡱�󶨵�ָ�����ڣ����Ѵ��ڵ�����������Ϊ��ǰ�̵߳���������
    glfwMakeContextCurrent(window);

    //ÿ�����ڵ�����Сʱ���ûص�����
    glfwSetFramebufferSizeCallback(window,[](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        });

    //ÿ��������ʱ���ûص�����
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
        });

    //ÿ���ƶ����ʱ���ûص�����
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xposIn, double yposIn) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        //������һ��xoffset��yoffset��Ϊ0�����½��봰�ں����һ��
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        //����ÿ֡����ƶ����벢����
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
        });

    //���ع��
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //��ʼ��GLAD GLAD����������OpenGL�ĺ���ָ���
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    stbi_set_flip_vertically_on_load(false);//��ת����
    glEnable(GL_DEPTH_TEST);//������Ȳ��ԣ�Z-Bufferʵ��

    return window;
}

//����Ƿ�����ESC
void processInput(GLFWwindow* window)
{
    //������ʱ��
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    //ESC�˳�
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    //�������
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

//��������
unsigned int loadTexture(char const* path)
{
    //��������
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        //ͨ��
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        //���ز���������
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); //��һ��0��Mipmap����
        glGenerateMipmap(GL_TEXTURE_2D);//Mipmap
        //Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //�ͷ�
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}

//�������chatgpt
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

//����ʱ��仯
void lampchange(GLFWwindow* window)
{
    float adjustTime = glfwGetTime() - pauseTime;//�ֲ���ͣ������ʱ��ƫ��         
    if (!isPause) {
        //�����������ٶȣ������ǰ뾶
        glm::vec3 vec = glm::vec3(1.0f);
        vec.x = 3.5f * cos(adjustTime * 1.0f);
        vec.y = 12.5f + 4.0f * sin(adjustTime * 1.5f) / 2.0f;
        vec.z = 3.5f * sin(adjustTime * 1.0f);
        lightPos = vec;
    }
    //����������һ֡״̬��ͬ�Ż�����
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !lastKeyState) {
        isPause = !isPause;
        lastKeyState = true; // ����״̬Ϊ����
        if (isPause) {
            // ��ʼ��ͣ����¼��ǰ��ʱ��
            pauseStartTime = glfwGetTime();
        }
        else {
            // ������ͣ��������ͣ��ʱ��
            pauseTime += glfwGetTime() - pauseStartTime;
        }
    }
    //ֱ���ɿ����������°���״̬
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        lastKeyState = false;
    }
}

float vertices[] = {
    //λ��
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