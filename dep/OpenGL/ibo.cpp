#include "error.h"
#include "ibo.h"

IBO::IBO() : ID(0), count(0) {}
IBO::IBO(const unsigned int* indices, unsigned int count) : count(count) {
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));
	GLCall(glGenBuffers(1, &ID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
}
IBO::~IBO() { GLCall(glDeleteBuffers(1, &ID)); }
void IBO::bind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID)); }
void IBO::unbind() const { GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }