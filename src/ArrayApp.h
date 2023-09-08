#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Application.h"
#include "OpenGL/vao.h"
#include "OpenGL/vbo.h"
#include "OpenGL/ibo.h"
#include "OpenGL/texture.h"
#include "OpenGL/shader.h"

class ArrayApp : public Application {
public:
	ArrayApp(unsigned int width, unsigned int height);
	~ArrayApp() {
		m_vertexArray.reset();
		m_vertexBuffer.reset();
		m_indexBuffer.reset();
		m_texture.reset();
		m_shader.reset();
		m_data.clear();
		m_resultData.clear();
	}
	void render();
	void renderImGui();
	void resize(unsigned int width, unsigned int height);
	virtual void update(float deltaTime) {}
	virtual void initData() {}
protected:
	std::vector<unsigned char> m_data, m_resultData;
	size_t m_size;
	std::unique_ptr<VAO> m_vertexArray;
	std::unique_ptr<VBO> m_vertexBuffer;
	std::unique_ptr<IBO> m_indexBuffer;
	std::unique_ptr<Texture> m_texture;
	std::unique_ptr<Shader> m_shader;
};