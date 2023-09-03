#pragma	once

#include <vector>

struct VBElement {
	VBElement(unsigned int type, unsigned int count, unsigned int normalized) : type(type), count(count), normalized(normalized) {}
	unsigned int type, count, normalized;
	static unsigned int getSizeOfType(unsigned int type);
};

class VBL {
public:
	VBL() : stride(0) {}
	void push(unsigned int type, unsigned int count);
	inline unsigned int getStride() const { return stride; }
	inline const std::vector<VBElement>& getElements() const { return elements; }
private:
	std::vector<VBElement> elements;
	unsigned int stride;
};