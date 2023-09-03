#include "GameOfLife.h"
#include <imgui.h>
#include "OpenGL/error.h"
#include "OpenGL/vbl.h"
#include "OpenGL/renderer.h"

GameOfLife::GameOfLife(size_t width, size_t height) : Application(width, height), m_size(width * height) {
    std::cout << "GameOfLife()" << std::endl;
    m_name = "Game of Life";
    m_data = new unsigned char[m_size];
    m_resultData = new unsigned char[m_size];
    for (size_t i = 0; i < m_size; ++i) {
		m_data[i] = rand() % 2 ? 1 : 0;
		m_resultData[i] = 0;
	}

    auto vertexShaderSource = "#version 330 core\n"
        "layout(location = 0) in vec2 aPos;"
        "layout(location = 1) in vec2 aTexCoord;"
        "out vec2 texCoord;"
        "void main() {"
        "gl_Position = vec4(aPos, 0.f, 1.f);"
        "texCoord = aTexCoord;"
        "}";
    auto fragmentShaderSource = "#version 330 core\n"
		"out vec4 color;"
		"in vec2 texCoord;"
		"uniform sampler2D tex;"
		"void main() {"
		"color = texture(tex, texCoord);"
		"color = 255.f * vec4(color.xxx, 1.f);"
		"}";
    GLfloat points[] = {
        -1.f, -1.f, 0.f, 0.f,
        -1.f, 1.f, 0.f, 1.f,
        1.f, -1.f, 1.f, 0.f,
        1.f, 1.f, 1.f, 1.f
    };
    GLuint indices[] = { 0, 1, 2, 1, 2, 3 };

	m_vertexArray = std::make_unique<VAO>();
    m_vertexBuffer = std::make_unique<VBO>(points, sizeof(points));
    VBL layout;
    layout.push(GL_FLOAT, 2);
    layout.push(GL_FLOAT, 2);
    m_vertexArray->addBuffer(*m_vertexBuffer, layout);
    m_indexBuffer = std::make_unique<IBO>(indices, 6);
    m_texture = std::make_unique<Texture>(m_width, m_height, m_data, GL_RED, GL_RED);
    m_shader = std::make_unique<Shader>("src/GameOfLife.shader");
    m_shader->bind();
    m_shader->setUniform1i("tex", 0);
}

GameOfLife::~GameOfLife() {
	delete[] m_data;
	delete[] m_resultData;
}

void GameOfLife::render() {
    //std::cout << "Rendering" << std::endl;
    /*for (int i = 0; i < m_size; i++) {
		std::cout << (int)m_data[i];
        if (i % m_width == m_width - 1) std::cout << std::endl;
	}*/
    GLCall(glClearColor(0.f, 0.f, 0.f, 0.f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    m_texture->bind();
    m_shader->bind();
    Renderer renderer;
    renderer.draw(*m_vertexArray, *m_indexBuffer, *m_shader);
}

void GameOfLife::renderImGui() {
    ImGui::Text("Frame time: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void GameOfLife::resize(int width, int height) {
    std::cout << "Resizing to " << width << "x" << height << std::endl;
    m_width = width;
    m_height = height;
    m_size = width * height;
    delete[] m_data;
    delete[] m_resultData;
    m_data = new unsigned char[m_size];
    m_resultData = new unsigned char[m_size];
    for (int i = 0; i < m_size; i++) {
        m_data[i] = rand() % 2;
    }
    m_texture = std::make_unique<Texture>(m_width, m_height, m_data, GL_RED, GL_RED);
    m_texture->bind();
    m_shader->bind();
    m_shader->setUniform1i("tex", 0);
}