#include "error.h"
#include "shader.h"

Shader::Shader() : ID(-1) {}
Shader::Shader(std::string str) {
	ShaderSource src = readShader(str);
	std::cout << "Vertex source: " << std::endl << src.vert << std::endl << std::endl;
	std::cout << "Fragment source: " << std::endl << src.frag << std::endl << std::endl;
	createShader(src);
}
Shader::Shader(std::string strv, std::string strf, bool isPath) {
	ShaderSource src;
	if (isPath) src = readShader(strv, strf);
	else src = { strv, strf };
	std::cout << "Vertex source: " << std::endl << src.vert << std::endl << std::endl;
	std::cout << "Fragment source: " << std::endl << src.frag << std::endl << std::endl;
	createShader(src);
}
Shader::~Shader() { GLCall(glDeleteProgram(ID)); }

void Shader::bind() const { GLCall(glUseProgram(ID)); }
void Shader::unbind() const { GLCall(glUseProgram(0)); }

ShaderSource Shader::readShader(std::string path) {
	std::ifstream stream(path);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) type = VERTEX;
			else if (line.find("fragment") != std::string::npos) type = FRAGMENT;
		}
		else ss[(int)type] << line << '\n';
	}
	return { ss[0].str(), ss[1].str() };
}

ShaderSource Shader::readShader(std::string vertexPath, std::string fragmentPath) {
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile, fShaderFile;
	vShaderFile.open(vertexPath);
	fShaderFile.open(fragmentPath);
	std::stringstream vShaderStream, fShaderStream;
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();
	vShaderFile.close();
	fShaderFile.close();
	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();
	std::string vShaderCode = vertexCode.c_str();
	std::string fShaderCode = fragmentCode.c_str();
	return { vShaderCode, fShaderCode };
}

unsigned int Shader::compileShader(unsigned int type, std::string src) {
	GLCall(unsigned int id = glCreateShader(type));
	const char* src_c = src.c_str();
	GLCall(glShaderSource(id, 1, &src_c, NULL));
	GLCall(glCompileShader(id));
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)_malloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

unsigned int Shader::createShader(ShaderSource src) {
	ID = glCreateProgram();
	unsigned int vertex = compileShader(GL_VERTEX_SHADER, src.vert);
	unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, src.frag);
	GLCall(glAttachShader(ID, vertex));
	GLCall(glAttachShader(ID, fragment));
	GLCall(glLinkProgram(ID));
	GLint program_linked;
	GLCall(glGetProgramiv(ID, GL_LINK_STATUS, &program_linked));
	std::cout << "Program link status: " << program_linked << std::endl;
	if (program_linked != GL_TRUE) {
		GLsizei log_length = 0;
		GLchar message[1024];
		GLCall(glGetProgramInfoLog(ID, 1024, &log_length, message));
		std::cout << "Failed to link program" << std::endl;
		std::cout << message << std::endl;
	}
	GLCall(glValidateProgram(ID));
	GLCall(glDeleteShader(vertex));
	GLCall(glDeleteShader(fragment));
	return ID;
}

int Shader::getUniformLocation(const std::string& name) const {
	if (locationCache.find(name) != locationCache.end()) return locationCache[name];
	GLCall(int location = glGetUniformLocation(ID, name.c_str()));
	if (location == -1) std::cout << "[Warning] Uniform variable '" << name << "' does not exist" << std::endl;
	locationCache[name] = location;
	return location;
}

void Shader::setUniform1i(const std::string& name, int v0) const { glUniform1i(getUniformLocation(name), v0); }
void Shader::setUniform2i(const std::string& name, int v0, int v1) const { glUniform2i(getUniformLocation(name), v0, v1); }
void Shader::setUniform1f(const std::string& name, float v0) const { glUniform1f(getUniformLocation(name), v0); }
void Shader::setUniform2f(const std::string& name, float v0, float v1) const { glUniform2f(getUniformLocation(name), v0, v1); }
void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2) const { glUniform3f(getUniformLocation(name), v0, v1, v2); }
void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const { glUniform4f(getUniformLocation(name), v0, v1, v2, v3); }
void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) const { glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]); }