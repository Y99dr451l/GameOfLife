#pragma once
#ifdef USE_STB_IMAGE
#include "stb/stb_image.h"
#endif

class Texture {
public:
	Texture();
	Texture(size_t newWidth, size_t newHeight, const unsigned char* newData, GLuint format1, GLuint format2);
#ifdef USE_STB_IMAGE
	Texture(const std::string& path);
#endif
	~Texture();
	void bind(unsigned int slot = 0) const;
	void unbind() const;
	inline size_t getWidth() const { return width; }
	inline size_t getHeight() const { return height; }
private:
	unsigned int ID;
	std::string path;
	unsigned char* localBuffer;
	size_t width, height, BPP;
};