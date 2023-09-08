#pragma once
#include <vector>
#include <string>
#include <functional>

class Application {
public:
	Application() {}
	Application(unsigned int width, unsigned int height) : m_name("Application"), m_width(width), m_height(height), m_x(0), m_y(0), m_zoom(1.0f) {}
	virtual void update(float deltaTime) {}
	virtual void render() {}
	virtual void renderImGui() {}
	virtual const char* getName() { return m_name.c_str(); }
	virtual void resize(unsigned int width, unsigned int height) {
		m_width = width;
		m_height = height;
	}
	virtual void move(float x, float y) {
		m_x += x * m_zoom;
		m_y += y * m_zoom;
	}
	virtual void zoom(float zFactor) {
		m_zoom *= zFactor;
		if (m_zoom < 0.1f) m_zoom = 0.1f;
		if (m_zoom > 10.0f) m_zoom = 10.0f;
	}
	void addOffset(float x, float y) {
		m_x -= x / m_width * m_zoom;
		m_y += y / m_height * m_zoom;
	}
protected:
	unsigned int m_width, m_height;
	std::string m_name;
	float m_x, m_y, m_zoom;
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
	Application* getCurrentApp() { return m_currentApp; }
private:
	Application*& m_currentApp;
	std::vector<std::pair<std::string, std::function<Application*()>>> m_apps;
};