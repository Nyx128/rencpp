#include "rcp_io.hpp"
#include <stdexcept>

namespace rcp {
	std::string readTextFile(std::string filePath){
		std::ifstream file(filePath.c_str(), std::ios::ate);
		if (!file.is_open()) {
			std::string err = "failed to read file at (";
			err += filePath;
			err += ")";
			throw std::runtime_error(err);
		}
		std::vector<char> buffer;
		buffer.resize(file.tellg());
		file.seekg(0);
		file.read(buffer.data(), buffer.size());
		file.close();
		std::string text(buffer.begin(), buffer.end());
		return text;
	}
}