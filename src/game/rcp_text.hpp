#pragma once
#include "backend/rcp_font.hpp"
#include <memory>
#include "glad/glad.h"

namespace rcp {
	class Text {
	public:
		Text(const Text&) = delete;
		Text& operator=(const Text&) = delete;

		Text(std::shared_ptr<rcp::Font>& _font, std::string& _body, uint32_t _fontSize);
		~Text();

	private:
		std::shared_ptr<rcp::Font>& font;
		
		std::string body;
		uint32_t fontSize;
	};
}