
#include <iostream>
#include "OpenGL/OpenGL.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Application.h"
#include "SerialCPU.h"
#include "ParallelCPU.h"
#include "BasicGPU.cuh"

#define WIDTH 800
#define HEIGHT 600

static AppLoader* menu = nullptr;
static double dragStartX, dragStartY;
static bool isDragging = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	menu->resize(width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	float zFactor = 1.f - yoffset * 0.1f;
	menu->getCurrentApp()->zoom(zFactor);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			isDragging = true;
			glfwGetCursorPos(window, &dragStartX, &dragStartY);
		}
		else if (action == GLFW_RELEASE) isDragging = false;
	}
}

void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
	if (isDragging) {
		menu->getCurrentApp()->addOffset(xpos - dragStartX, ypos - dragStartY);
		dragStartX = xpos;
		dragStartY = ypos;
	}
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Game of Life", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSwapInterval(0);
	gladLoadGL();

	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui::StyleColorsDark();
		ImGui::GetIO().IniFilename = NULL;

		Application* currentApp = nullptr;
		menu = new AppLoader(WIDTH, HEIGHT, currentApp);
		currentApp = menu;
		menu->addApp<SerialCPU>("Serial CPU");
		menu->addApp<ParallelCPU>("Parallel CPU");
		menu->addApp<BasicGPU>("Basic GPU");

		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT);
			glfwPollEvents();
			processInput(window);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			if (currentApp) {
				currentApp->update(0.f);
				currentApp->render();
				ImGui::Begin(currentApp->getName());
				if (currentApp != menu && ImGui::Button("<-")) {
					delete currentApp;
					currentApp = menu;
				}
				currentApp->renderImGui();
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(window);
		}

		delete currentApp;
		if (currentApp != menu) delete menu;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}