#pragma once
#include "ft2build.h"
#include FT_FREETYPE_H

#include <string>
#include <unordered_map>
#include <array>

#include "glm/glm.hpp"
#include "glad/glad.h"

namespace rcp {
	/// <summary>
	/// my approach to handling font abstration from freetype will be as such:
	/// 1. Each font is going to have its differnt FT_Library handle
	/// 2. each rcp::Font handle will have different faces of the same font which are set to different font sizes, provided if there is need for them
	/// 3. it will keep a map of characters with corresponding textures
	/// </summary>
	class Font {
	public:
		Font(const Font&) = delete;
		Font& operator=(const Font&) = delete;

		Font(std::string _fontPath, int _faceIndex = 0);
		~Font();

		struct Character {
			GLuint texture;
			glm::ivec2 size;
			glm::ivec2 bearing;
			uint32_t advance;
		};

		std::array<Character, 128>& getCharacterMap(int pixelSize);
		FT_Face& getFace(int pixelSize);
		
	private:
		int faceIndex;

		std::string fontPath;
		FT_Library lib;
		std::unordered_map<int, FT_Face> face_map;

		void loadFace(int pixelSize);

		std::unordered_map<int, std::array<Character, 128>> character_maps;
	};
}
