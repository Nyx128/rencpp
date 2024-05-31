#pragma once
#include "glad/glad.h"
#include "rcp_image.hpp"

#include <memory>

namespace rcp {

	struct TextureDescription {
		GLuint wrapMode;
		GLuint minFilter;
		GLuint magFilter;
	};

	class Texture {
	public:
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		Texture(TextureDescription _desc);
		~Texture();

		void loadImage(std::shared_ptr<rcp::Image>& image);
		void bind(GLuint texSlot);

	private:
		GLuint handle;
		TextureDescription desc;
	};
}
