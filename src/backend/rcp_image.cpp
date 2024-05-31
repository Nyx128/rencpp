#include "rcp_image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <stdexcept>

namespace rcp {
	Image::Image(std::string _imagePath, bool flipVertical):imagePath(_imagePath){
		stbi_set_flip_vertically_on_load(flipVertical);
		pixels = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);
		if (!pixels) {
			std::string err = "failed to load image from file (";
			err += imagePath;
			err += ")";
			stbi_image_free(pixels);
			throw std::runtime_error(err);
		}

	}

	Image::~Image(){
		stbi_image_free(pixels);
	}
}