#pragma once

class IBO {
public:
	IBO();
	IBO(const unsigned int* indices, unsigned int count);
	~IBO();
	void bind() const;
	void unbind() const;
	inline unsigned int getCount() const { return count; }
private:
	unsigned int ID, count;
};