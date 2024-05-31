#pragma once
#include "glad/glad.h"
#include <stdint.h>
#include <string>

namespace rcp {
	class Image {
	public:

		Image(const Image&) = delete;
		Image& operator=(const Image&) = delete;

		Image(std::string _imagePath, bool flipVertical = false);
		~Image();

		int getWidth() { return width; }
		int getHeight() { return height; }
		int getChannels() { return channels; }

		uint8_t* getPixels() { return pixels; }
	private:
		uint8_t* pixels;
		int width, height, channels;

		std::string imagePath;
	};
}