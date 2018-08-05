#include "graphics\vertex_array.h"
#include "GL\glew.h"

namespace graphics {
	vertex_array::vertex_array() {
		glGenVertexArrays(1, &m_render_id);
		glBindVertexArray(m_render_id);
	}

	vertex_array::~vertex_array() {
		glDeleteVertexArrays(1, &m_render_id);

		for (std::vector<vertex_buffer*>::iterator it = _buffers.begin(); it != _buffers.end(); ++it) {
			delete (*it);
		}

		for (std::vector<vertex_buffer_layout*>::iterator it = _layouts.begin(); it != _layouts.end(); ++it) {
			delete (*it);
		}

		_buffers.clear();
		_layouts.clear();
	}

	void vertex_array::AddBuffer(vertex_buffer* vb, vertex_buffer_layout* layout) {
		bind();
		vb->bind();
		const auto& elements = layout->GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++) {
			const auto& element = elements[i];
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, element.count, element.type, element.normalized, layout->GetStride(), (const void*)offset);
			offset += element.count * vertex_buffer_layout_element::sizeof_type(element.type);
		}

		_buffers.push_back(vb);
		_layouts.push_back(layout);
	}

	void vertex_array::bind() const {
		glBindVertexArray(m_render_id);
	}

	void vertex_array::unbind() const {
		glBindVertexArray(0);
	}
}