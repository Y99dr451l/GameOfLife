#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>

struct ShaderSource { std::string vert, frag; };

enum ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

class Shader {
public:
	Shader();
	Shader(std::string str);
	Shader(std::string strv, std::string strf, bool isPath = true);
	~Shader();

	void bind() const;
	void unbind() const;
	ShaderSource readShader(std::string path);
	ShaderSource readShader(std::string vertexPath, std::string fragmentPath);
	unsigned int compileShader(unsigned int type, std::string src);
	unsigned int createShader(ShaderSource src);

	int getUniformLocation(const std::string& name) const;
	void setUniform1i(const std::string& name, int v0) const;
	void setUniform2i(const std::string& name, int v0, int v1) const;
	void setUniform1f(const std::string& name, float v0) const;
	void setUniform2f(const std::string& name, float v0, float v1) const;
	void setUniform3f(const std::string& name, float v0, float v1, float v2) const;
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix) const;

private:
	unsigned int ID;
	mutable std::unordered_map<std::string, int> locationCache;
};