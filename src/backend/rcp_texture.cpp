#include "rcp_texture.hpp"

namespace rcp {
	Texture::Texture(TextureDescription _desc):desc(_desc){
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, desc.wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, desc.wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, desc.minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, desc.magFilter);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::~Texture(){
		glDeleteTextures(1, &handle);
	}

	void Texture::loadImage(std::shared_ptr<rcp::Image>& image){
		glBindTexture(GL_TEXTURE_2D, handle);
		if (image->getChannels() == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(), image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->getPixels());
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		if (image->getChannels() == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->getWidth(), image->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, image->getPixels());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::bind(GLuint texSlot){
		glActiveTexture(texSlot);
		glBindTexture(GL_TEXTURE_2D, handle);
	}
}