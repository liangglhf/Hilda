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
    //���ڳ�ʼ���ͻ�������
    GLFWwindow* window = Init();
    
    //ʵ������ɫ��
    Shader ourShader("Res/Shader/model.vs", "Res/Shader/model.fs");
    Shader lightcubeShader("Res/Shader/light_cube.vs", "Res/Shader/light_cube.fs");

    //ʵ����ģ��
    //Model ourModel("res/Texture/Furina/Furina.pmx");
    Model ourModel("res/Texture/Furina/Furina_Black.pmx");

    //VA0,VBO
    unsigned int VBO, cubeVAO, lightcubeVAO;
    //����
    glGenVertexArrays(1, &cubeVAO);
    glGenVertexArrays(1, &lightcubeVAO);
    glGenBuffers(1, &VBO);
    //����
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//�������ݸ��Ƶ�������ڴ���
    //lightcubeλ������                             
    glBindVertexArray(lightcubeVAO);                
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//������������
    glEnableVertexAttribArray(0);//���ö�������     

    //------------------------------------------------------------------------------------
    //��Ⱦѭ��
    while (!glfwWindowShouldClose(window))//���������ر�ǰ��Ҫ�ر�
    {
        error();//����
        processInput(window);// ����
        lampchange(window);//��λ����ʱ��仯

        //�����Ļ
        //glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//ī��
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Z-Buffer
        
        //������ɫ��
        ourShader.use();
        //����ɫ������������ɫ��Ԫ��Ӧ
        //���
        ourShader.setVec3("viewPos", camera.GetCameraPosition());
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
        ourShader.setMat4("model", model);
        //�ƵĹ�
        ourShader.setVec3("lampLight.position", lightPos);
        ourShader.setVec3("lampLight.ambient", 0.5f, 0.5f, 0.5f); 
        ourShader.setVec3("lampLight.diffuse", 3.0f, 3.0f, 3.0f); 
        ourShader.setVec3("lampLight.specular", 5.0f, 5.0f, 5.0f);
        //�ӽǹ�
        ourShader.setVec3("spotLight.position", camera.GetCameraPosition());
        ourShader.setVec3("spotLight.direction", camera.GetCameraFront());//���Ĺ�Դλ��Ϊ���λ��
        ourShader.setVec3("spotLight.ambient", 0.5f, 0.5f, 0.5f);  
        ourShader.setVec3("spotLight.diffuse", 5.0f, 5.0f, 5.0f);  
        ourShader.setVec3("spotLight.specular", 5.0f, 5.0f, 5.0f);
        ourShader.setFloat("spotLight.constant", 1.0f);
        ourShader.setFloat("spotLight.linear", 0.02f);
        ourShader.setFloat("spotLight.quadratic", 0.005f); 
        ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(7.5f)));
        ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(10.0f)));

        ourModel.Draw(ourShader);

        //��
        lightcubeShader.use();//���ܷ���ǰ�棬openglֻ��ʶ��ǰ�������ɫ��
        lightcubeShader.setMat4("projection", projection);
        lightcubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.4f));
        lightcubeShader.setMat4("model", model);
        glBindVertexArray(lightcubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ��鲢�����¼�����������
        glfwSwapBuffers(window);//ÿ֡������ɺ󣬰�׼���õ�ͼ����ʾ����Ļ��,������˸
        glfwPollEvents();//�����û�д���ʲô�¼�
    }

    //���
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //ɾ��
    glDeleteVertexArrays(1, &lightcubeVAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}