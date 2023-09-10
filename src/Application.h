#pragma once
#include <vector>
#include <string>
#include <functional>

class Application {
public:
	Application() {}
	Application(unsigned int width, unsigned int height) : m_name("Application"), m_width(width), m_height(height), m_offsetX(0), m_offsetY(0), m_zoom(1.0f) {}
	virtual void update(float deltaTime) {}
	virtual void render() {}
	virtual void renderImGui() {}
	inline const char* getName() { return m_name.c_str(); }
	virtual void resize(unsigned int width, unsigned int height) {
		m_width = width;
		m_height = height;
	}
	virtual void move(float x, float y) {
		m_offsetX += x * m_zoom;
		m_offsetY += y * m_zoom;
	}
	virtual void zoom(float zFactor) {
		m_zoom *= zFactor;
		if (m_zoom < 0.1f) m_zoom = 0.1f;
		if (m_zoom > 10.0f) m_zoom = 10.0f;
	}
	void addOffset(float x, float y) {
		m_offsetX -= x / m_width * m_zoom;
		m_offsetY += y / m_height * m_zoom;
	}
	void setCursorPos(float x, float y) {
		m_mouseX = x;
		m_mouseY = y;
	}
protected:
	unsigned int m_width, m_height;
	std::string m_name;
	float m_offsetX, m_offsetY, m_mouseX, m_mouseY, m_zoom;
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
	inline Application* getCurrentApp() { return m_currentApp; }
private:
	Application*& m_currentApp;
	std::vector<std::pair<std::string, std::function<Application*()>>> m_apps;
};