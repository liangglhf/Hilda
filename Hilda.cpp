#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/vendor/glm/glm.hpp"
#include "src/vendor/glm/gtc/matrix_transform.hpp"
#include "src/vendor/glm/gtc/type_ptr.hpp"

#include <iostream>
#include "Shader.h"
#include "func.h"

int main()
{
    //窗口初始化和基础设置
    GLFWwindow* window = Init();
    
    //实例化着色器
    Shader ourShader("Res/Shader/model.vs", "Res/Shader/model.fs");
    Shader lightcubeShader("Res/Shader/light_cube.vs", "Res/Shader/light_cube.fs");

    //实例化模型
    //Model ourModel("res/Texture/Furina/Furina.pmx");
    Model ourModel("res/Texture/Furina/Furina_Black.pmx");

    //VA0,VBO
    unsigned int VBO, cubeVAO, lightcubeVAO;
    //创建
    glGenVertexArrays(1, &cubeVAO);
    glGenVertexArrays(1, &lightcubeVAO);
    glGenBuffers(1, &VBO);
    //设置
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//顶点数据复制到缓冲的内存中
    //lightcube位置设置                             
    glBindVertexArray(lightcubeVAO);                
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//解析顶点数据
    glEnableVertexAttribArray(0);//启用顶点属性     

    //------------------------------------------------------------------------------------
    //渲染循环
    while (!glfwWindowShouldClose(window))//在我主动关闭前不要关闭
    {
        error();//纠错
        processInput(window);// 输入
        lampchange(window);//灯位置随时间变化

        //清空屏幕
        //glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//墨绿
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Z-Buffer
        
        //激活着色器
        ourShader.use();
        //将着色器采样器和颜色单元对应
        //相机
        ourShader.setVec3("viewPos", camera.GetCameraPosition());
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
        ourShader.setMat4("model", model);
        //灯的光
        ourShader.setVec3("lampLight.position", lightPos);
        ourShader.setVec3("lampLight.ambient", 0.5f, 0.5f, 0.5f); 
        ourShader.setVec3("lampLight.diffuse", 3.0f, 3.0f, 3.0f); 
        ourShader.setVec3("lampLight.specular", 5.0f, 5.0f, 5.0f);
        //视角光
        ourShader.setVec3("spotLight.position", camera.GetCameraPosition());
        ourShader.setVec3("spotLight.direction", camera.GetCameraFront());//更改光源位置为相机位置
        ourShader.setVec3("spotLight.ambient", 0.5f, 0.5f, 0.5f);  
        ourShader.setVec3("spotLight.diffuse", 5.0f, 5.0f, 5.0f);  
        ourShader.setVec3("spotLight.specular", 5.0f, 5.0f, 5.0f);
        ourShader.setFloat("spotLight.constant", 1.0f);
        ourShader.setFloat("spotLight.linear", 0.02f);
        ourShader.setFloat("spotLight.quadratic", 0.005f); 
        ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(7.5f)));
        ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(10.0f)));

        ourModel.Draw(ourShader);

        //灯
        lightcubeShader.use();//不能放在前面，opengl只会识别当前激活的着色器
        lightcubeShader.setMat4("projection", projection);
        lightcubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.4f));
        lightcubeShader.setMat4("model", model);
        glBindVertexArray(lightcubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 检查并调用事件，交换缓冲
        glfwSwapBuffers(window);//每帧绘制完成后，把准备好的图像显示到屏幕上,避免闪烁
        glfwPollEvents();//检查有没有触发什么事件
    }

    //解绑
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //删除
    glDeleteVertexArrays(1, &lightcubeVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}