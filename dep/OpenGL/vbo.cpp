#include "error.h"
#include "vbo.h"

VBO::VBO() : ID(0) {}
VBO::VBO(const void* data, size_t size) {
	GLCall(glGenBuffers(1, &ID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
VBO::~VBO() {
	GLCall(glDeleteBuffers(1, &ID));
}
void VBO::bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
}
void VBO::unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}	