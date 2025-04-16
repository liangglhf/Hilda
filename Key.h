#pragma once
#include "func.h"

class Key {
public:
    Key(GLFWwindow* window);
    //灯,球随时间变化
    void lampchange(GLFWwindow* window);
    void spotlighting(GLFWwindow* window);
private:
    GLFWwindow* window;
};