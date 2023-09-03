#include "error.h"
#include "vbl.h"

unsigned int VBElement::getSizeOfType(unsigned int type) {
	switch (type) {
	case GL_FLOAT: return sizeof(GLfloat);
	case GL_UNSIGNED_INT: return sizeof(GLuint);
	case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
	}
	ASSERT(false);
	return 0;
}

void VBL::push(unsigned int type, unsigned int count) {
	elements.push_back(VBElement(type, count, (type == GL_UNSIGNED_BYTE) ? GL_TRUE : GL_FALSE));
	stride += count * VBElement::getSizeOfType(type);
}