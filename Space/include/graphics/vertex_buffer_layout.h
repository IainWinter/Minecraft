#pragma once
#include <vector>
#include "GL\glew.h"

namespace graphics {
	struct vertex_buffer_layout_element {
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int sizeof_type(unsigned int gl_type);
	};

	class vertex_buffer_layout {
	private:
		std::vector<vertex_buffer_layout_element> m_elements;
		unsigned int m_stride;
	public:
		vertex_buffer_layout() : m_stride(0) {}

		template<typename T>
		void Push(unsigned int count) {
			static_assert(false);
		}

		template<>
		void Push<float>(unsigned int count) {
			m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
			m_stride += count * vertex_buffer_layout_element::sizeof_type(GL_FLOAT);
		}

		template<>
		void Push<unsigned int>(unsigned int count) {
			m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			m_stride += count * vertex_buffer_layout_element::sizeof_type(GL_UNSIGNED_INT);
		}

		template<>
		void Push<unsigned char>(unsigned int count) {
			m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			m_stride += count * vertex_buffer_layout_element::sizeof_type(GL_UNSIGNED_BYTE);
		}

		inline const std::vector<vertex_buffer_layout_element>& GetElements() const {
			return m_elements;
		}

		inline unsigned int GetStride() const {
			return m_stride;
		}
	};
}