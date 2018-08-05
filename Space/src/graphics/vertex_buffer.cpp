#include "graphics\vertex_buffer.h"
#include "GL\glew.h"

namespace graphics {
	vertex_buffer::vertex_buffer(const void* data, unsigned int size) : m_data(data) {
		glGenBuffers(1, &m_render_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_render_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	vertex_buffer::~vertex_buffer() {
		glDeleteBuffers(1, &m_render_id);
		delete m_data;
	}

	void vertex_buffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_render_id);
	}

	void vertex_buffer::unbind() const {
		(GL_ARRAY_BUFFER, 0);
	}
}