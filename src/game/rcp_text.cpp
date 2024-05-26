#include "rcp_text.hpp"
#include <stdexcept>

namespace rcp {
	Text::Text(std::shared_ptr<rcp::Font>& _font, std::string& _body, uint32_t _fontSize):font(_font), body(_body), fontSize(_fontSize){
		
	}

	Text::~Text(){

	}
}