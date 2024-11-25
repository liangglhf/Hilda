#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    :Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    //lookAt矩阵，把视角移到相机位置，三个参数：相机位置，目标位置，世界空间向上向量
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
    //四参数：视角，宽高比，near，far
    return glm::perspective(glm::radians(Zoom), (float)m_width / (float)m_height, 0.1f, 100.0f);
}

glm::vec3 Camera::GetCameraPosition()
{
    return Position;
}

glm::vec3 Camera::GetCameraFront()
{
    return Front;
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    //控制鼠标移动灵敏度
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    //限制偏航角
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    //更新相机的三维方向
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}