#pragma once
#include "src/vendor/imgui/imgui.h"
#include "src/vendor/imgui/imgui_impl_glfw.h"
#include "src/vendor/imgui/imgui_impl_opengl3.h"
#include "src/vendor/glm/glm.hpp"
#include "src/vendor/glm/gtc/matrix_transform.hpp"
#include "src/vendor/glm/gtc/type_ptr.hpp"

//#include "camera.h"
#include "func.h"
#include <string>

class imguiSet {
public:
	imguiSet(GLFWwindow* window);
	~imguiSet();
	void newframe();
	void set();
	void render();
private:
	const char* glsl_version = "#version 330 core";
	GLFWwindow* window;
public:
	ImVec4 ClearColor = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);
};