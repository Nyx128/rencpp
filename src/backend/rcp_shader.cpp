#include "rcp_shader.hpp"
#include <fstream>
#include <vector>
#include <filesystem>

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

		vLastWrite = std::filesystem::last_write_time(vPath);
		fLastWrite = std::filesystem::last_write_time(fPath);
	}

	Shader::~Shader(){
		glDeleteProgram(handle);
	}
	void Shader::reload(){
		auto vtime = std::filesystem::last_write_time(vPath);
		auto ftime = std::filesystem::last_write_time(fPath);

		if (vtime != vLastWrite || ftime != fLastWrite) {
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

			vLastWrite = vtime;
			fLastWrite = ftime;
		}
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
}