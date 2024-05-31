#include "rcp_shader.hpp"
#include <fstream>
#include <vector>
#include <filesystem>

#include "glm/gtc/type_ptr.hpp"

namespace rcp {
	Shader::Shader(std::string v, std::string f):vPath(v), fPath(f){
		std::string vSrc = readFile(vPath);
		std::string fSrc = readFile(fPath);

		const char* vcSrc = vSrc.c_str();
		const char* fcSrc = fSrc.c_str();

		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vShader, 1, &vcSrc, NULL);
		glCompileShader(vShader);

		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fShader, 1, &fcSrc, NULL);
		glCompileShader(fShader);

		handle = glCreateProgram();
		glAttachShader(handle, vShader);
		glAttachShader(handle, fShader);
		glLinkProgram(handle);

		glDeleteShader(vShader);
		glDeleteShader(fShader);
	}

	Shader::~Shader(){
		glDeleteProgram(handle);
	}
	void Shader::updateMat4(std::string name, glm::mat4 val){
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(val));
	}

	void Shader::updateVec2(std::string name, glm::vec2 val){
		glUniform2f(getUniformLocation(name), val.x, val.y);
	}

	void Shader::updateFloat(std::string name, float val){
		glUniform1f(getUniformLocation(name), val);
	}

	void Shader::updateInt(std::string name, int val){
		glUniform1i(getUniformLocation(name), val);
	}

	void Shader::getFloat(std::string name, float* val){
		glGetUniformfv(handle, getUniformLocation(name), val);
	}

	void Shader::reload(){
		glDeleteProgram(handle);
		std::string vSrc = readFile(vPath);
		std::string fSrc = readFile(fPath);

		const char* vcSrc = vSrc.c_str();
		const char* fcSrc = fSrc.c_str();

		GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vShader, 1, &vcSrc, NULL);
		glCompileShader(vShader);

		GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fShader, 1, &fcSrc, NULL);
		glCompileShader(fShader);

		handle = glCreateProgram();
		glAttachShader(handle, vShader);
		glAttachShader(handle, fShader);
		glLinkProgram(handle);

		glDeleteShader(vShader);
		glDeleteShader(fShader);
	}

	void Shader::bind(){
		glUseProgram(handle);
	}

	std::string Shader::readFile(std::string path){
		std::ifstream file(path.c_str(), std::ios::ate);
		if (!file.is_open()) {
			std::string err = "failed to load shader file at (";
			err += path;
			err += ")";
			throw std::runtime_error(err);
		}
		size_t size = file.tellg();
		file.seekg(0);
		std::vector<char> buffer;
		buffer.resize(size);
		file.read(buffer.data(), size);
		file.close();
		return std::string(buffer.begin(), buffer.end());
	}

	GLuint Shader::getUniformLocation(std::string val){
		if (uniformMap.find(val) != uniformMap.end()) {
			return uniformMap[val];
		}
		else {
			GLuint loc = glGetUniformLocation(handle, val.c_str());
			if (loc == UINT32_MAX) { throw std::runtime_error("trying to access uniform that doesnt exist"); }
			uniformMap[val] = loc;
			return loc;
		}
	}
}