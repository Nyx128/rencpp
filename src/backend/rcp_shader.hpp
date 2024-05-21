#pragma once
#include "glad/glad.h"
#include <string>
#include <filesystem>

namespace rcp {
	/// <summary>
	/// basic shader abstraction for vertex and fragment.
	/// loads shader source source files and builds a shader program
	/// reloading feature, reload shaders while the program runs
	/// intended to be used as a shared pointer
	/// </summary>
	class Shader {
	public:
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		Shader(std::string v, std::string f);
		~Shader();

		void reload();
		void bind();
	private:
		GLuint handle;

		std::string vPath;
		std::string fPath;

		std::string readFile(std::string path);

		//for reloading checks
		std::filesystem::file_time_type vLastWrite;
		std::filesystem::file_time_type fLastWrite;
	};
}
