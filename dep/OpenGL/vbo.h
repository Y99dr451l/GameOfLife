#pragma once

class VBO {
public:
	VBO();
	VBO(const void* data, size_t size);
	~VBO();
	void bind() const;
	void unbind() const;
private:
	unsigned int ID;
};