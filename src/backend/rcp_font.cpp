#include "rcp_font.hpp"
#include <stdexcept>

namespace rcp {
	Font::Font(std::string _fontPath, int _faceIndex):fontPath(_fontPath), faceIndex(_faceIndex){
		auto error = FT_Init_FreeType(&lib);

		if (error != 0) {
			throw std::runtime_error("Failed to initialize freetype library");
		}


	}

	Font::~Font(){
		for (auto& face : face_map) {
			FT_Done_Face(face.second);
		}
		FT_Done_FreeType(lib);
	}

	void Font::loadFace(int pixelSize){
		face_map.emplace(pixelSize, FT_Face());

		FT_Face& face = face_map.at(pixelSize);
		auto error = FT_New_Face(lib, fontPath.c_str(), faceIndex, &face);
		FT_Set_Pixel_Sizes(face, pixelSize, pixelSize);

		if (error != 0) {
			std::string err = "failed to load font from file at (";
			err += fontPath;
			err += ") with face index: ";
			err += std::to_string(faceIndex);
			err += "and with pixel size: ";
			err += std::to_string(pixelSize);
			throw std::runtime_error(err);
		}
	}

	FT_Face& Font::getFace(int pixelSize){
		//ideally you want this to always hit, preload as much as possible
		if (face_map.find(pixelSize) != face_map.end()) {
			return face_map[pixelSize];
		}
		else {
			loadFace(pixelSize);
			return face_map[pixelSize];
		}
	}

	std::array<Font::Character, 128>& Font::getCharacterMap(int pixelSize) {
		if (character_maps.find(pixelSize) != character_maps.end()) {
			return character_maps[pixelSize];
		}
		//otherwise load it and then give it back
		else {
			std::array<Character, 128> char_map;

			FT_Face& face = getFace(pixelSize);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			for (uint32_t c = 0; c < 128; c++) {
				if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
					throw std::runtime_error("Failed to load character map");
				}

				GLuint texture;
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
				);
				// set texture options
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				char_map[c].texture = texture;
				char_map[c].size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
				char_map[c].bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
				char_map[c].advance = face->glyph->advance.x;
			}
			character_maps.emplace(pixelSize, char_map);
			return character_maps[pixelSize];
		}
	}
}