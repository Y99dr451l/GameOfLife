#include "Application.h"
#include <imgui.h>

AppLoader::AppLoader(unsigned int width, unsigned int height, Application*& app) : Application(width, height), m_currentApp(app) { m_name = "Menu"; }

void AppLoader::renderImGui() {
	for (auto& app : m_apps)
		if (ImGui::Button(app.first.c_str())) m_currentApp = app.second();
}