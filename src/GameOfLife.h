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

class GameOfLife : public Application {
public:
	GameOfLife(size_t width, size_t height);
	void render();
	void renderImGui();
	void resize(int width, int height);
protected:
    inline unsigned char countAliveCells(size_t x0, size_t x1, size_t x2, size_t y0, size_t y1, size_t y2) {
        return m_data[x0 + y0] + m_data[x1 + y0] + m_data[x2 + y0] + m_data[x0 + y1] + m_data[x2 + y1] + m_data[x0 + y2] + m_data[x1 + y2] + m_data[x2 + y2];
    }
	std::vector<unsigned char> m_data, m_resultData;
	size_t m_size;
	std::unique_ptr<VAO> m_vertexArray;
	std::unique_ptr<VBO> m_vertexBuffer;
	std::unique_ptr<IBO> m_indexBuffer;
	std::unique_ptr<Texture> m_texture;
	std::unique_ptr<Shader> m_shader;
};