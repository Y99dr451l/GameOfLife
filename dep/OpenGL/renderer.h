#pragma once

#include "OpenGL/OpenGL.h"
#include "vao.h"
#include "ibo.h"
#include "shader.h"
#include "error.h"

class Renderer {
public:
	void draw(const VAO& vao, const IBO& ibo, const Shader& shader) const;
	void clear() const;
private:

};