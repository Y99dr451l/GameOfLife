#include "ArrayApp.h"
#include <imgui.h>
#include "OpenGL/error.h"
#include "OpenGL/vbl.h"
#include "OpenGL/renderer.h"

ArrayApp::ArrayApp(unsigned int width, unsigned int height) : Application(width, height), m_size(width * height) {
    m_name = "Array Application";

    auto vSource = "#version 330 core\n"
        "layout(location = 0) in vec2 aPos;"
        "layout(location = 1) in vec2 aTexCoord;"
        "out vec2 texCoord;"
        "void main() {"
        "gl_Position = vec4(aPos, 0.f, 1.f);"
        "texCoord = aTexCoord;"
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
}

void ArrayApp::render() {
    //GLCall(glClearColor(0.f, 0.f, 0.f, 0.f));
    //GLCall(glClear(GL_COLOR_BUFFER_BIT));
    m_texture->bind();
    m_shader->bind();
    Renderer renderer;
    renderer.draw(*m_vertexArray, *m_indexBuffer, *m_shader);
}

void ArrayApp::renderImGui() {
    ImGui::Text("Frame time: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void ArrayApp::resize(unsigned int width, unsigned int height) {
    std::cout << "Resizing to " << width << "x" << height << std::endl;
    m_width = width;
    m_height = height;
    m_size = width * height;
    this->initData();
    if (m_data.size() == m_size) m_texture = std::make_unique<Texture>(m_width, m_height, m_data.data(), GL_RED, GL_RED);
    m_texture->bind();
    m_shader->bind();
    m_shader->setUniform1i("tex", 0);
}