#include "graphics/index_buffer.h"
#include "GL\glew.h"

namespace graphics {
	index_buffer::index_buffer(const void* data, unsigned int count) : m_count(count), m_data(data) {
		glGenBuffers(1, &m_render_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_render_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
	}

	index_buffer::~index_buffer() {
		glDeleteBuffers(1, &m_render_id);
		delete m_data;
	}

	void index_buffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_render_id);
	}

	void index_buffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void index_buffer::draw() const {
		glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, nullptr);
	}
}