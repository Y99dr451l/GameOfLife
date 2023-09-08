#pragma once
#ifdef USE_STB_IMAGE
#include "stb/stb_image.h"
#endif

class Texture {
public:
	Texture();
	Texture(GLsizei newWidth, GLsizei newHeight, GLint format1 = 0, GLenum format2 = 0, GLenum type = 0);
#ifdef USE_STB_IMAGE
	Texture(const std::string& path);
#endif
	~Texture();
	void setData(const unsigned char* newData);
	void setSize(GLsizei newWidth, GLsizei newHeight);
	void setFilters(GLint minFilter, GLint magFilter = 0);
	void setWrapping(GLint sWrapping, GLint tWrapping = 0);
	void updateMipmaps();
	void bind(unsigned int slot = 0) const;
	void unbind() const;
	inline GLsizei getWidth() const { return m_width; }
	inline GLsizei getHeight() const { return m_height; }
private:
	unsigned int m_ID;
	GLsizei m_width, m_height;
	GLint m_intFormat;
	GLenum m_format, m_type;
#ifdef USE_STB_IMAGE
	std::string m_path;
	unsigned char* m_localBuffer;
	size_t m_BPP;
#endif
};