#include "error.h"
#include "texture.h"

Texture::Texture() {
	GLCall(glGenTextures(1, &m_ID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
	unbind();
}
Texture::Texture(GLsizei width, GLsizei height, GLint intFormat, GLenum format, GLenum type) {
	GLCall(glGenTextures(1, &m_ID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
	setSize(width, height);
	if (intFormat) m_intFormat = intFormat; else m_intFormat = GL_RGBA;
	if (format) m_format = format; else m_format = GL_RGBA;
	if (type) m_type = type; else m_type = GL_UNSIGNED_BYTE;
}
#ifdef USE_STB_IMAGE
Texture::Texture(const std::string& path) : path(path), localBuffer(nullptr), width(0), height(0), BPP(0) {
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(path.c_str(), &width, &height, &BPP, 4);
	GLCall(glGenTextures(1, &ID));
	GLCall(glBindTexture(GL_TEXTURE_2D, ID));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	if (localBuffer) stbi_image_free(localBuffer);
}
#endif
Texture::~Texture() { unbind(); GLCall(glDeleteTextures(1, &m_ID)); }

void Texture::bind(unsigned int slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
}
void Texture::unbind() const { GLCall(glBindTexture(GL_TEXTURE_2D, 0)); }

void Texture::setData(const unsigned char* newData) {
	bind();
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, m_intFormat, m_width, m_height, 0, m_format, m_type, (const void*)newData));
}
void Texture::setSize(GLsizei newWidth, GLsizei newHeight) {
	m_width = newWidth;
	m_height = newHeight;
}
void Texture::setFilters(GLint minFilter, GLint magFilter) {
	if (!magFilter) magFilter = minFilter;
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
}
void Texture::setWrapping(GLint sWrapping, GLint tWrapping) {
	if (!tWrapping) tWrapping = sWrapping;
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrapping));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrapping));
}
void Texture::updateMipmaps() { GLCall(glGenerateMipmap(GL_TEXTURE_2D)); }