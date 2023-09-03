#pragma once

#include "vbo.h"
#include "vbl.h"

class VAO {
public:
	VAO();
	~VAO();
	void bind() const;
	void unbind() const;
	void addBuffer(const VBO& vb, const VBL& layout);
private:
	unsigned int ID;
};