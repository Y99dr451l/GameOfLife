#include "ArrayApp.h"
#include <imgui.h>
#include "OpenGL/error.h"
#include "OpenGL/vbl.h"
#include "OpenGL/renderer.h"

ArrayApp::ArrayApp(unsigned int width, unsigned int height) : Application(width, height), m_size(width * height) {
    m_name = "Array Application";
    m_x = 0.5f, m_y = 0.5f;

    auto vSource = "#version 330 core\n"
        "layout(location = 0) in vec2 aPos;"
        "layout(location = 1) in vec2 aTexCoord;"
        "out vec2 texCoord;"
        "uniform float zoom;"
        "uniform vec2 offset;"
        "void main() {"
        "gl_Position = vec4(aPos, 0.f, 1.f);"
        "texCoord = (aTexCoord - vec2(.5f)) * zoom + vec2(.5f) + offset;"
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
}

void ArrayApp::render() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    m_texture->bind();
    m_shader->bind();
    m_shader->setUniform1f("zoom", m_zoom);
    m_shader->setUniform2f("offset", m_x, m_y);
    Renderer renderer;
    renderer.draw(*m_vertexArray, *m_indexBuffer, *m_shader);
}

void ArrayApp::renderImGui() {
    ImGui::SameLine();
    float fr = ImGui::GetIO().Framerate;
    ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / fr, fr);
    ImGui::Text("Offset: x = %.3f, y = %.3f", m_x, m_y);
    ImGui::Text("Zoom: %.3f", m_zoom);
    if (ImGui::Button("Randomise field")) initData();
    ImGui::SameLine();
    if (ImGui::Button("Reset view")) {
		m_x = 0.5f, m_y = 0.5f;
		m_zoom = 1.f;
	}
}

void ArrayApp::resize(unsigned int width, unsigned int height) {
    std::cout << "Resizing to " << width << "x" << height << std::endl;
    m_width = width, m_height = height;
    m_size = width * height;
    m_texture->setSize(width, height);
    initData();
}