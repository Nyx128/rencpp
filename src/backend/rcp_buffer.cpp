#include "rcp_buffer.hpp"
#include <stdexcept>

namespace rcp {
	Buffer::Buffer(GLenum target, GLenum usage, size_t size, void* _data):buf_target(target), buf_usage(usage), buf_size(size), data(_data){
		glGenBuffers(1, &handle);
		bind();
		glBufferData(target, buf_size, data, buf_usage);
		glBindBuffer(target, 0);
	}

	Buffer::~Buffer(){
		clean();
	}

	void Buffer::update(size_t offset, size_t size, void* _data){
		if (offset + size > buf_size) {
			throw std::runtime_error("Attempting to write to buffer beyond its memory range");
		}
		bind();
		glBufferSubData(buf_target, offset, size, _data);
	}

	void Buffer::clean(){
		glDeleteBuffers(1, &handle);
	}
}