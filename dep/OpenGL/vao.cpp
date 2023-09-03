#include "error.h"
#include "vao.h"

VAO::VAO() { GLCall(glGenVertexArrays(1, &ID)); }
VAO::~VAO() { GLCall(glDeleteVertexArrays(1, &ID)); }
void VAO::bind() const { GLCall(glBindVertexArray(ID)); }
void VAO::unbind() const { GLCall(glBindVertexArray(0)); }
void VAO::addBuffer(const VBO& vb, const VBL& layout) {
	bind();
	vb.bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));
		offset += element.count * VBElement::getSizeOfType(element.type);
	}
}