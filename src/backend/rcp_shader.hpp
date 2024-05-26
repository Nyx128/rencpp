#pragma once
#include "glad/glad.h"
#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

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

		void updateMat4(std::string name, glm::mat4 val);
		void updateVec2(std::string name, glm::vec2 val);
		void updateFloat(std::string name, float val);
		void updateInt(std::string name, int val);

		void reload();
		void bind();
	private:
		GLuint handle;

		std::string vPath;
		std::string fPath;

		std::string readFile(std::string path);

		//for uniform locations
		std::unordered_map<std::string, GLuint> uniformMap;
		GLuint getUniformLocation(std::string val);
	};
}
