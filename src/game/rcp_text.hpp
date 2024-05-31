#pragma once

#include "backend/rcp_buffer.hpp"
#include "backend/rcp_font.hpp"
#include "backend/rcp_shader.hpp"

#include <memory>
#include "glad/glad.h"

#include "glm/glm.hpp"


namespace rcp {
	class Text {
	public:
		Text(const Text&) = delete;
		Text& operator=(const Text&) = delete;

		Text(std::shared_ptr<rcp::Font>& _font, std::string& _body, uint32_t _fontSize, float _scale, float _lineSpace);
		~Text();

		void render(glm::vec3 position);

	private:
		std::shared_ptr<rcp::Font>& font;
		
		std::string body;
		uint32_t fontSize;
		float scale;

		rcp::Buffer vbo{ GL_ARRAY_BUFFER, GL_STATIC_DRAW, sizeof(float) * 36, nullptr };
		GLuint vao;

		float lineSpace;

		rcp::Shader textShader{ "shaders/def/text.vert", "shaders/def/text.frag" };


		float timer = 0.0f;
	};
}