#pragma once
#include "vertex_buffer_layout.h"
#include "vertex_buffer.h"

namespace graphics {
	class vertex_array {
	private:
		unsigned int m_render_id;
		std::vector<vertex_buffer*> _buffers;
		std::vector<vertex_buffer_layout*> _layouts;
	public:
		vertex_array();
		~vertex_array();

		void AddBuffer(vertex_buffer* vb, vertex_buffer_layout* layout);

		void bind() const;
		void unbind() const;
	};
}