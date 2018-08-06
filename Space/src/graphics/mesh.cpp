#include "graphics\mesh.h"
#include "graphics\index_buffer.h"
#include "graphics\mesh_factory.h"
#include "GL\glew.h"
#include "IwMath\matrix4.h"

namespace graphics {
	mesh::mesh(graphics::vertex_array* va, index_buffer* ib)
		: m_vertex_array(va), m_index_buffer(ib) {
	}

	mesh::~mesh() {
		//delete &m_vertex_array;
		delete m_index_buffer;
	}

	void mesh::draw(const iwmath::vector3& position, const iwmath::quaternion& rotation) const {
		iwmath::matrix4 transform = iwmath::matrix4::create_from_quaternion(rotation) * iwmath::matrix4::create_translation(position);

		glUniformMatrix4fv(8, 1, GL_FALSE, transform.elements);

		m_vertex_array->bind();
		m_index_buffer->bind();

		m_index_buffer->draw();
	}

	mesh* mesh::create_sphere(const float& scale, const unsigned int& resolution) {
		return mesh_factory::create_sphere(resolution);
	}
}