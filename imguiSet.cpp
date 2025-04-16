#include "imguiSet.h"

imguiSet::imguiSet(GLFWwindow* window)
    :window(window)
{
	ImGui::CreateContext();//创建imgui上下文
	ImGui_ImplGlfw_InitForOpenGL(window, true);//初始化glfw
	ImGui_ImplOpenGL3_Init(glsl_version);//初始化opengl,版本为opengl 3.3
	ImGui::StyleColorsDark();//颜色
}

imguiSet::~imguiSet()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void imguiSet::newframe()
{
	ImGui_ImplOpenGL3_NewFrame();//为 OpenGL 渲染系统初始化当前帧
	ImGui_ImplGlfw_NewFrame();//一定要在前一步之后，用来处理输入事件的，需要正确的窗口上下文来获取输入事件
	ImGui::NewFrame();//开始新的 ImGui 渲染帧
	//注意，初始化的时候先初始化glfw还是opengl都可以，但是newframe的时候一定要先opengl
}

void imguiSet::set()
{
    //imgui面板
    ImGui::Begin("Furina");
    //相机速度
    if (ImGui::Button("CameraSpeed0")) camera.MovementSpeed = SPEED;
    ImGui::SameLine();// 下条保持在同一行
    ImGui::SliderFloat("CameraSpeed", &camera.MovementSpeed, 5.0f, 20.0f);
    //灯速度
    if (ImGui::Button("LampSpeed0")) lightSpeed = glm::vec3(1.0f, 1.5f, 1.0f);
    ImGui::SameLine();
    ImGui::SliderFloat3("LampSpeed", &lightSpeed.x, 0.0f, 5.0f);//0和5是滑条最小和最大值
    //球折射率
    if (ImGui::Combo("Material", &curMaterial, materials, IM_ARRAYSIZE(materials))) {
        ratio = 1.0f / IOR[curMaterial];
    }
    ImGui::SliderFloat("Manual Ratio", &ratio, 1.0f / 2.5f, 1.0f / 1.0f);
    //背景色
    ImGui::ColorEdit3("clear color", (float*)&ClearColor);
    if (ImGui::Button("shadow black")) ClearColor = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
    ImGui::SameLine();
    if (ImGui::Button("dark green")) ClearColor = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);
    //边框
    ImGui::Checkbox("lockLight", &lockLight);//是否锁定灯
    //草和窗户
    ImGui::SameLine();
    ImGui::Checkbox("grass", &grass);
    ImGui::SameLine();
    ImGui::Checkbox("window", &windowPNG);
    //地板
    ImGui::Checkbox("plane", &plane);
    ImGui::SameLine();
    ImGui::SliderFloat("planeHeight", &planeHeight, 0.0f, 5.0f);
    // 一些图像效果
    ImGui::Text("post processing");
    ImGui::Checkbox("sharpen", &issharpen);// 锐化
    ImGui::SameLine();
    ImGui::Checkbox("edge", &isedge);// 边缘检测
    ImGui::SameLine();
    ImGui::Checkbox("emboss", &isemboss);// 浮雕
    ImGui::SameLine();
    ImGui::Checkbox("embossInvert", &isembossInvert);// 反转浮雕
    ImGui::SameLine();
    ImGui::Checkbox("solarize", &issolarize);// 太阳化
    ImGui::SameLine();
    ImGui::Checkbox("blur", &isblur);// 高斯模糊
    // 帧率 我也不知道有什么用，看着cool吧
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //结束
    ImGui::End();//有begin就必须有end，可以都不写，不写begin的话默认窗口名字叫debug
}

void imguiSet::render()
{
    ImGui::Render();//渲染imgui
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());//通过imgui渲染图形
}
