#include "GameOfLife.h"
#include <imgui.h>
#include "OpenGL/error.h"
#include "OpenGL/vbl.h"
#include "OpenGL/renderer.h"

GameOfLife::GameOfLife(unsigned int width, unsigned int height) : Application(width, height), m_size(width * height) {
    m_name = "Array Application";
    m_offsetX = 0.5f, m_offsetY = 0.5f;

    auto vSource = "#version 330 core\n"
        "layout(location = 0) in vec2 aPos;"
        "layout(location = 1) in vec2 aTexCoord;"
        "out vec2 texCoord;"
        "uniform float zoom;"
        "uniform vec2 offset;"
        "void main() {"
        "gl_Position = vec4(aPos, 0.f, 1.f);"
        "texCoord = (aTexCoord - vec2(.5f)) * zoom + offset;"
        "}";
    auto fSource = "#version 330 core\n"
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
    m_shader = std::make_unique<Shader>(vSource, fSource, false);
    m_shader->bind();
    m_shader->setUniform1i("tex", 0);

    m_texture = std::make_unique<Texture>(m_width, m_height, GL_RED, GL_RED, GL_UNSIGNED_BYTE);
    m_texture->bind();
    m_texture->setFilters(GL_LINEAR, GL_NEAREST);
    m_texture->setWrapping(GL_REPEAT);
    GLCall(glClearColor(0.f, 0.f, 0.f, 0.f));

    m_selectedPattern = nullptr;
    m_patterns.push_back({ "Glider", 3, 3, { 0, 0, 1, 1, 0, 1, 0, 1, 1 } });
    m_patterns.push_back({ "Lightweight", 5, 4, { 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0 } });
}

void GameOfLife::render() {
    if (m_data.size() == m_size) m_texture->setData(m_data.data());
    if (m_selectedPattern) {
        unsigned int w = m_selectedPattern->width, h = m_selectedPattern->height;
        auto io = ImGui::GetIO();
        float mx = io.MousePos.x, my = m_height - io.MousePos.y;
        float x = (mx / m_width - .5f) * m_zoom + m_offsetX;
        float y = (my / m_height - .5f) * m_zoom + m_offsetY;
        x = (int)(x * m_width) % m_width, y = (int)(y * m_height) % m_height;
        m_texture->bind();
        glTexSubImage2D(GL_TEXTURE_2D, 0, (int)x, (int)y, w, h, GL_RED, GL_UNSIGNED_BYTE, m_selectedPattern->data.data());
        if (ImGui::IsMouseClicked(1)) {
            for (unsigned int i = 0; i < w; ++i)
                for (unsigned int j = 0; j < h; ++j) {
                    unsigned int index = (int)x + i + ((int)y + j) * m_width;
					m_data[index] = m_selectedPattern->data[i + j * w];
				}
		}
	}
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    m_texture->bind();
    m_shader->bind();
    m_shader->setUniform1f("zoom", m_zoom);
    m_shader->setUniform2f("offset", m_offsetX, m_offsetY);
    Renderer renderer;
    renderer.draw(*m_vertexArray, *m_indexBuffer, *m_shader);
}

void GameOfLife::renderImGui() {
    ImGui::Text("Pan with LMB\nZoom with mouse wheel\nPlace with RMB");
    ImGui::Separator();

    auto io = ImGui::GetIO();
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Text("Window position:\n x = %.3f\n y = %.3f", io.MousePos.x, io.MousePos.y);
    float mx = io.MousePos.x, my = m_height - io.MousePos.y;
    float x = (mx / m_width - .5f) * m_zoom + m_offsetX;
    float y = (my / m_height - .5f) * m_zoom + m_offsetY;
    x = (int)(x * m_width) % m_width, y = (int)(y * m_height) % m_height;
    ImGui::Text("Texture position:\n x = %.3f\n y = %.3f", x, y);
    ImGui::Separator();

    ImGui::Text("Offset:\n x = %.3f\n y = %.3f", m_offsetX, m_offsetY);
    ImGui::Text("Zoom: %.3f", m_zoom);
    if (ImGui::Button("Randomise field")) initData();
    ImGui::SameLine();
    if (ImGui::Button("Reset view")) {
		m_offsetX = 0.5f, m_offsetY = 0.5f;
		m_zoom = 1.f;
	}
    ImGui::Separator();

    ImGui::Text("Selected pattern: %s", m_selectedPattern ? m_selectedPattern->name.c_str() : "None");
    if (ImGui::BeginListBox("")) {
        for (auto& pattern : m_patterns) {
            bool selected = &pattern == m_selectedPattern;
            if (ImGui::Selectable(pattern.name.c_str(), selected)) m_selectedPattern = &pattern;
        }
        ImGui::EndListBox();
    }
}

void GameOfLife::resize(unsigned int width, unsigned int height) {
    std::cout << "Resizing to " << width << "x" << height << std::endl;
    m_width = width, m_height = height;
    m_size = width * height;
    m_texture->setSize(width, height);
    initData();
}