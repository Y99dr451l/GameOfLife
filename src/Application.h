#pragma once
#include <vector>
#include <string>
#include <functional>

class Application {
public:
	Application() { m_name = "Application"; }
	Application(unsigned int width, unsigned int height) : m_width(width), m_height(height) { m_name = "Application"; }
	virtual ~Application() {}
	virtual void update(float deltaTime) {}
	virtual void render() {}
	virtual void renderImGui() {}
	virtual const char* getName() { return m_name.c_str(); }
	virtual void resize(unsigned int width, unsigned int height) {
		m_width = width;
		m_height = height;
	}
protected:
	unsigned int m_width, m_height;
	std::string m_name;
};

class AppLoader : public Application {
public:
	AppLoader(unsigned int width, unsigned int height, Application*& app);
	void renderImGui();
	void resize(unsigned int width, unsigned int height) {
		Application::resize(width, height);
		if (m_currentApp != this) m_currentApp->resize(width, height);
	}
	template<typename T>
	void addApp(const std::string& name) {
		m_apps.push_back(std::make_pair(name, [=]() { return new T(m_width, m_height); }));
	}
private:
	Application*& m_currentApp;
	std::vector<std::pair<std::string, std::function<Application*()>>> m_apps;
};