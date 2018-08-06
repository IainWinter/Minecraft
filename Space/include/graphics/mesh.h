#pragma once
#include "vertex_array.h"
#include "index_buffer.h"
#include "vertex_buffer_layout.h"
#include "IwMath\vector3.h"
#include "IwMath\quaternion.h"

namespace graphics {
	class mesh {
	private:
		vertex_array* m_vertex_array;
		index_buffer* m_index_buffer;
	public:
		mesh(vertex_array* va, index_buffer* ib);
		~mesh();

		void draw(const iwmath::vector3& position, const iwmath::quaternion& rotation) const;

		static mesh* create_sphere(const float& scale, const unsigned int& resolution);
	};
}