#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "src/vendor/glm/glm.hpp"
#include "src/vendor/glm/gtc/matrix_transform.hpp"
#include "src/vendor/glm/gtc/type_ptr.hpp"

#include <iostream>
#include <map>
#include "imguiSet.h"
#include "Key.h"

int main()
{
    //窗口初始化和基础设置
    GLFWwindow* window = Init();
    //按键设置
    Key key(window);
    
    //实例化着色器
    Shader ourShader("Res/Shader/model.vs", "Res/Shader/model.fs");
    Shader lightcubeShader("Res/Shader/light_cube.vs", "Res/Shader/light_cube.fs");
    Shader single_colorShader("Res/Shader/single_color.vs", "Res/Shader/single_color.fs");
    Shader grassShader("Res/Shader/other.vs", "Res/Shader/grass.fs");
    Shader otherShader("Res/Shader/other.vs", "Res/Shader/other.fs");
    Shader screenShader("Res/Shader/screen.vs", "Res/Shader/screen.fs");
    Shader skyboxShader("Res/Shader/skybox.vs", "Res/Shader/skybox.fs");
    Shader ballShader("Res/Shader/ball.vs", "Res/Shader/ball.fs");

    //实例化模型
    //Model ourModel("res/Texture/Furina/Furina.pmx");
    Model ourModel("res/Texture/Furina/Furina_Black.pmx");

    //VAO,VBO   不要只用一个VBO，不好管理，容易出错
    //灯
    //创建
    unsigned int lightcubeVAO, lightcubeVBO;
    glGenVertexArrays(1, &lightcubeVAO);
    glGenBuffers(1, &lightcubeVBO);
    //绑定
    glBindVertexArray(lightcubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightcubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    //顶点设置
    glEnableVertexAttribArray(0);//启用顶点属性  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);//解析顶点数据
    //地板
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //草
    unsigned int grassVAO, grassVBO;
    glGenVertexArrays(1, &grassVAO);
    glGenBuffers(1, &grassVBO);
    glBindVertexArray(grassVAO);
    glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gwVertices), gwVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //窗户
    unsigned int windowVAO, windowVBO;
    glGenVertexArrays(1, &windowVAO);
    glGenBuffers(1, &windowVBO);
    glBindVertexArray(windowVAO);
    glBindBuffer(GL_ARRAY_BUFFER, windowVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gwVertices), gwVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //球
    generateBall();
    unsigned int ballVAO, ballVBO;
    glGenVertexArrays(1, &ballVAO);
    glGenBuffers(1, &ballVBO);
    glBindVertexArray(ballVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ballVBO);
    glBufferData(GL_ARRAY_BUFFER, generateBall().size() * sizeof(float), generateBall().data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //画中画
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    // 天空盒
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //设置帧缓冲
    //创建帧缓冲并绑定
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    //创建并设置纹理附件，作为帧缓冲的颜色附件
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);//附加到帧缓冲
    //创建并设置渲染缓冲对象附件，存储深度和模板数据
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);//附加到帧缓冲
    //检查帧缓冲是否完整
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);//绑定到0，默认缓冲帧，即在屏幕上渲染

    //加载纹理
    unsigned int planeTexture = loadTexture("res/Texture/containers/metal.png");
    unsigned int grassTexture = loadTexture("res/Texture/containers/grass.png");
    unsigned int windowTexture = loadTexture("res/Texture/containers/window.png");
    //天空盒
    unsigned int cubemapTexture = loadCubemap(faces);

    //测试
    glEnable(GL_DEPTH_TEST);//开启深度测试，Z-Buffer实现
    glDepthFunc(GL_LESS);//可删，该情况为默认情况，即在片段深度值小于缓冲的深度值时通过测试
    glEnable(GL_STENCIL_TEST);//开启模版测试，测试模版值
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);/*
    没有通过模版测试，keep保持当前储存的模板值
    通过模版测试，没有通过深度测试，replace将模板值设置为glStencilFunc函数设置的ref值(第二个参数)
    通过模版测试，通过深度测试，replace */  
    glEnable(GL_BLEND);//开启混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//确定混合比例

    //imgui
    imguiSet imgui(window);

    //------------------------------------------------------------------------------------
    //渲染循环
    while (!glfwWindowShouldClose(window))//在我主动关闭前不要关闭
    {
        imgui.newframe();

        error();//纠错
        processInput(window);// 输入
        key.lampchange(window);//球、灯位置随时间变化
        key.spotlighting(window);//视角光

        //渲染到帧缓冲
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST);
        //清空屏幕
        glClearColor(imgui.ClearColor.x, imgui.ClearColor.y, imgui.ClearColor.z, imgui.ClearColor.w);//背景颜色可设置
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//通过深度测试、模版测试  

        //寄存窗户与摄像机的距离，来决定窗户的渲染顺序，以确定遮挡关系
        std::map<float, glm::vec3> sorted;//map升序排序
        for (unsigned int i = 0; i < gw.size(); i++)
        {
            float distance = glm::length(camera.Position - gw[i]);//glm::length计算距离
            sorted[distance] = gw[i];//将距离作为键存入，值是点的位置
        }

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
        if (spotLight) { // 按下按钮开关视角光
            ourShader.setVec3("spotLight.position", camera.GetCameraPosition());
            ourShader.setVec3("spotLight.direction", camera.GetCameraFront());//更改光源位置为相机位置
            ourShader.setVec3("spotLight.ambient", 0.5f, 0.5f, 0.5f);
            ourShader.setVec3("spotLight.diffuse", 3.0f, 3.0f, 3.0f);
            ourShader.setVec3("spotLight.specular", 5.0f, 5.0f, 5.0f);
            ourShader.setFloat("spotLight.constant", 1.0f);
            ourShader.setFloat("spotLight.linear", 0.02f);
            ourShader.setFloat("spotLight.quadratic", 0.005f);
            ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(7.5f)));
            ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(10.0f)));
        }
        else{
            ourShader.setVec3("spotLight.position", camera.GetCameraPosition());
            ourShader.setVec3("spotLight.direction", camera.GetCameraFront());//更改光源位置为相机位置
            ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            ourShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
            ourShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
            ourShader.setFloat("spotLight.constant", 1.0f);
            ourShader.setFloat("spotLight.linear", 0.02f);
            ourShader.setFloat("spotLight.quadratic", 0.005f);
            ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(7.5f)));
            ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(10.0f)));
        }
        glStencilMask(0x00);//禁止更新Furina的模版值
        ourModel.Draw(ourShader); 

        //灯
        lightcubeShader.use();//不能放在前面，opengl只会识别当前激活的着色器
        lightcubeShader.setMat4("projection", projection);
        lightcubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.4f));
        lightcubeShader.setMat4("model", model);
        //模版设置
        glStencilFunc(GL_ALWAYS, 1, 0xFF);//对 对象中所有像素(0xFF)总是(always)通过模版测试
        glStencilMask(0xFF);//允许改写对象所有像素的模版值，改写为Func中的ref，即1
        //渲染
        glBindVertexArray(lightcubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);//类型，第几个顶点开始，数量

        //灯边框
        if (lockLight) {//当锁定灯，边框才会出现
            single_colorShader.use();//不能放在前面，opengl只会识别当前激活的着色器
            single_colorShader.setMat4("projection", projection);
            single_colorShader.setMat4("view", view);
            model = glm::mat4(1.0f);
            model = glm::translate(model, lightPos);
            model = glm::scale(model, glm::vec3(0.6f));
            single_colorShader.setMat4("model", model);
            //模版设置
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);//对 对象中所有像素(0xFF)不等于1的(notequal)通过模版测试
            glStencilMask(0x00);//对接下来渲染的对象，禁止更新其模版值
            glDisable(GL_DEPTH_TEST);//关闭深度测试，不然没有边框的效果，注释可验证
            //渲染
            glBindVertexArray(lightcubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            //渲染完成重置设置
            glEnable(GL_DEPTH_TEST);//重新开启深度测试
            glStencilFunc(GL_ALWAYS, 0, 0xFF);//对 对象中所有像素(0xFF)总是(always)通过模版测试
            glStencilMask(0xFF);//允许改写对象所有像素的模版值，改写为Func中的ref，即0
        }

        //球
        ballShader.use();
        ballShader.setVec3("cameraPos", camera.GetCameraPosition());
        ballShader.setFloat("ratio", ratio);
        ballShader.setMat4("projection", projection);
        ballShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, ballPos);
        model = glm::scale(model, glm::vec3(3.0f));
        ballShader.setMat4("model", model);
        glBindVertexArray(ballVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, generateBall().size() / 6);

        //天空盒
        glDepthFunc(GL_LEQUAL);//深度值<=当前深度缓冲 则渲染
        skyboxShader.use();
        glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.setInt("samplerCube", cubemapTexture);
        skyboxShader.setMat4("view", skyboxView);
        skyboxShader.setMat4("projection", projection);
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

        //将这些放在天空盒之后满足透视关系
        //草
        if (grass) {
            grassShader.use();
            grassShader.setInt("texture1", 0);
            grassShader.setMat4("projection", projection);
            grassShader.setMat4("view", view);
            glBindVertexArray(grassVAO);
            glBindTexture(GL_TEXTURE_2D, grassTexture);
            for (unsigned int i = 0; i < gw.size(); i++)
            {
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(2.0f, planeHeight, 5.0f));
                model = glm::scale(model, glm::vec3(10.0f));
                model = glm::translate(model, gw[i]);
                grassShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }

        //其他
        otherShader.use();
        otherShader.setInt("texture1", 0);
        otherShader.setMat4("projection", projection);
        otherShader.setMat4("view", view);
        //地板
        if (plane) {
            glBindVertexArray(planeVAO);
            glBindTexture(GL_TEXTURE_2D, planeTexture);
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, planeHeight, 0.0f));
            model = glm::scale(model, glm::vec3(10.0f));
            otherShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        //窗户
        if (windowPNG) {
            glBindVertexArray(windowVAO);
            glBindTexture(GL_TEXTURE_2D, windowTexture);
            for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
            {//map是升序，逆序遍历，则是先找距离远的
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, planeHeight, 0.0f));
                model = glm::scale(model, glm::vec3(10.0f));
                model = glm::translate(model, it->second);//it->second，即值，将模型矩阵平移到该位置，以渲染该位置窗户
                //it->first是键
                otherShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //渲染二维的一帧不需要深度测试
        glDisable(GL_DEPTH_TEST);
        //渲染主屏幕
        screenShader.use();
        screenShader.setInt("screenTexture", 0);
        model = glm::mat4(1.0f);
        screenShader.setMat4("model", model);
        screenShader.setKernel("kernel", kernel);
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //锐化
        if (issharpen) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-0.75, 0.25, 0));
            model = glm::scale(model, glm::vec3(0.25));
            screenShader.setMat4("model", model);
            screenShader.setKernel("kernel", sharpen);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        //边缘检测
        if (isedge) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-0.75, -0.25, 0));
            model = glm::scale(model, glm::vec3(0.25));
            screenShader.setMat4("model", model);
            screenShader.setKernel("kernel", edge);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        //浮雕
        if (isemboss) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-0.75, -0.75, 0));
            model = glm::scale(model, glm::vec3(0.25));
            screenShader.setMat4("model", model);
            screenShader.setKernel("kernel", emboss);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        //反转浮雕
        if (isembossInvert) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.75, 0.25, 0));
            model = glm::scale(model, glm::vec3(0.25));
            screenShader.setMat4("model", model);
            screenShader.setKernel("kernel", embossInvert);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        //太阳化
        if (issolarize) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.75, -0.25, 0));
            model = glm::scale(model, glm::vec3(0.25));
            screenShader.setMat4("model", model);
            screenShader.setKernel("kernel", solarize);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        //高斯模糊
        if (isblur) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.75, -0.75, 0));
            model = glm::scale(model, glm::vec3(0.25));
            screenShader.setMat4("model", model);
            screenShader.setKernel("kernel", blur);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        imgui.set();

        imgui.render();
        // 检查并调用事件，交换缓冲
        glfwSwapBuffers(window);//每帧绘制完成后，把准备好的图像显示到屏幕上,避免闪烁
        glfwPollEvents();//检查有没有触发什么事件
    }

    //解绑
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //删除
    glDeleteVertexArrays(1, &lightcubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteVertexArrays(1, &grassVAO);
    glDeleteVertexArrays(1, &windowVAO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &lightcubeVBO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &grassVBO);
    glDeleteBuffers(1, &windowVBO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &framebuffer);
    glfwTerminate();
    return 0;
}