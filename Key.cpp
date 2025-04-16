#include "Key.h"

Key::Key(GLFWwindow* window)
    :window(window){}

void Key::lampchange(GLFWwindow* window){
    float adjustTime = glfwGetTime() - pauseTime;//弥补暂停带来的时间偏移         
    if (!isPause) {
        //括号里面是速度，外面是半径
        glm::vec3 lightVec = glm::vec3(1.0f);
        lightVec.x = 3.5f * cos(adjustTime * lightSpeed.x);
        lightVec.y = 12.5f + 4.0f * sin(adjustTime * lightSpeed.y) / 2.0f;
        lightVec.z = 3.5f * sin(adjustTime * lightSpeed.z);
        lightPos = lightVec;
        glm::vec3 ballVec = glm::vec3(1.0f);
        ballVec.x = 12.5f * cos(adjustTime * ballSpeed.x);
        ballVec.y = 20.0f;
        ballVec.z = 12.5f * sin(adjustTime * ballSpeed.z);
        ballPos = ballVec;
    }
    //按下且与上一帧状态不同才会启动
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spaceLast) {
        isPause = !isPause;
        spaceLast = true; // 更新状态为按下
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
        spaceLast = false;
    }
}

void Key::spotlighting(GLFWwindow* window){
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !spotlightLast) {
        spotLight = !spotLight;
        spotlightLast = true; // 更新状态为按下
    }
    //直到松开按键，更新按键状态
    else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
        spotlightLast = false;
    }
}
