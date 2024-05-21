#pragma once
#include "glad/glad.h"

namespace rcp {
	/// <summary>
	/// abstraction around opengl buffers mainly used for array buffers, element array buffers, ssbos and such
	/// minimal abstraction for buffer creation and updating buffer data in range
	/// </summary>
	class Buffer {
	public:
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		Buffer(GLenum target, GLenum usage, size_t size, void* _data);
		~Buffer();

		inline void bind() { glBindBuffer(buf_target, handle); }
		void update(size_t offset, size_t size, void* _data);

	private:
		GLenum buf_target;
		GLenum buf_usage;
		size_t buf_size;
		void* data;

		GLuint handle;

		void clean();
	};
}
