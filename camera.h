#pragma once

#include "src/vendor/glm/glm.hpp"
#include "src/vendor/glm/gtc/matrix_transform.hpp"
#include "src/vendor/glm/gtc/type_ptr.hpp"

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

//Ĭ�ϲ���
//Ĭ�ϲ��������ڱ���ʱȷ�������Է���class����
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;
const unsigned int m_width = 800, m_height = 600;

class Camera
{
private:
    //�������
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    //ŷ����
    float Yaw;
    float Pitch;
    
    //�����������ά����ŷ���Ǳ任����ʽ����
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);//��������򣬼����-x����
        Right = glm::normalize(glm::cross(Front, WorldUp));//���-x���������������õ����z����
        Up = glm::normalize(glm::cross(Right, Front));//���y����
    }
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW, float pitch = PITCH);

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 GetViewMatrix();

    glm::mat4 GetProjectionMatrix();

    glm::vec3 GetCameraPosition();

    glm::vec3 GetCameraFront();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    void ProcessMouseScroll(float yoffset);
};