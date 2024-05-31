#include "rcp_text.hpp"
#include <stdexcept>
#include <chrono>

#include "glm/gtc/matrix_transform.hpp"

namespace rcp {
	Text::Text(std::shared_ptr<rcp::Font>& _font, std::string& _body, uint32_t _fontSize, float _scale, float _lineSpace)
		:font(_font), body(_body), fontSize(_fontSize), scale(_scale), lineSpace(_lineSpace){
		if (scale < 0.0f) {
			throw std::runtime_error("text scale is either too small or negative");
		}
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		vbo.bind();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, nullptr);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));
		glBindVertexArray(0);
	}

	Text::~Text(){
		glDeleteVertexArrays(1, &vao);
	}

	void Text::render(glm::vec3 position){
		auto char_map = font->getCharacterMap(fontSize);
		glm::vec2 char_pos = glm::vec2(position.x, position.y);

		float aspect = 9.0f / 16.0f;
		float halfWidth = 640.0f;
		glm::mat4 proj = glm::ortho(-halfWidth, halfWidth, aspect * -halfWidth, aspect * halfWidth, 0.01f, 60.0f);
		textShader.bind();
		textShader.updateMat4("transform", proj);
		
		glBindVertexArray(vao);
		glActiveTexture(GL_TEXTURE0);
		vbo.bind();

		float delay = 0.0f;
		for (char c : body) {
			if (c != '\n') {
				delay += 0.07f;

				auto character = char_map[c];

				float xpos = char_pos.x + character.bearing.x * scale;
				float ypos = char_pos.y - (character.size.y - character.bearing.y) * scale;

				float w = character.size.x * scale;
				float h = character.size.y * scale;

				float vertices[6][5] = {
					{ xpos,     ypos + h, position.z,   0.0f, 0.0f},
					{ xpos,     ypos,  position.z,     0.0f, 1.0f},
					{ xpos + w, ypos,  position.z,     1.0f, 1.0f},

					{ xpos,     ypos + h, position.z,  0.0f, 0.0f},
					{ xpos + w, ypos,    position.z,   1.0f, 1.0f },
					{ xpos + w, ypos + h,  position.z, 1.0f, 0.0f}
				};

				vbo.update(0, sizeof(float) * 30, vertices);

				glBindTexture(GL_TEXTURE_2D, character.texture);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				char_pos.x += (character.advance.x >> 6) * scale;
			}
			else {
				char_pos.x = position.x;
				char_pos.y -= lineSpace;
			}
		}
	}
}