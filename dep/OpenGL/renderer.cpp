#include <iostream>
#include "renderer.h"

void Renderer::draw(const VAO& vao, const IBO& ibo, const Shader& shader) const {
	shader.bind();
	vao.bind();
	ibo.bind();
	GLCall(glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr));
}
void Renderer::clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}