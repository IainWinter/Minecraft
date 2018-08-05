#include "graphics\mesh.h"
#include "graphics\index_buffer.h"
#include "GL\glew.h"
#include "IwMath\matrix4.h"

namespace graphics {
	mesh::mesh(graphics::vertex_array* va, index_buffer* ib)
		: m_vertex_array(va), m_index_buffer(ib) {
	}

	mesh::~mesh() {
		delete m_vertex_array;
		delete m_index_buffer;
	}

	void mesh::draw(const iwmath::vector3& position, const iwmath::quaternion& rotation) const {
		iwmath::matrix4 world = iwmath::matrix4::create_from_quaternion(rotation) * iwmath::matrix4::create_translation(position);

		glUniformMatrix4fv(0, 1, GL_FALSE, world.elements);

		m_vertex_array->bind();
		m_index_buffer->bind();

		m_index_buffer->draw();
	}

	mesh* mesh::create_sphere(const float& scale, const unsigned int& resolution) {
		unsigned int vertCount =   2 * resolution * resolution;
		unsigned int indexCount =  3 * vertCount;

		iwmath::vector3* verts = new iwmath::vector3[vertCount];
		unsigned int*    index = new unsigned int[indexCount];

		float endU = 2 * IW_PI;
		float endV = IW_PI;
		float stepU = endU / resolution;
		float stepV = endV / resolution;
		for (unsigned int i = 0; i < resolution; i++) {
			for (unsigned int j = 0; j < resolution; j++) {
				float u = i * stepU;
				float v = j * stepV;
				float un = i + 1 == resolution ? endU : (i + 1) * stepU;
				float vn = j + 1 == resolution ? endV : (j + 1) * stepV;

				//Get points of quad

				iwmath::vector3 p0 = F(u, v, scale);
				verts[i + j * resolution] = p0;

				index[i + j * resolution] = i + j * resolution;
				index[i + j * resolution] = i + j * resolution;
				index[i + j * resolution] = i + j * resolution;

			}
		}

		graphics::vertex_buffer_layout* vbl = new graphics::vertex_buffer_layout();
		vbl->Push<float>(3);

		graphics::vertex_array* va = new graphics::vertex_array();
		graphics::index_buffer* ib = new graphics::index_buffer(index, indexCount);

		graphics::vertex_buffer* vb = new graphics::vertex_buffer(verts, sizeof(iwmath::vector3) * vertCount);
		va->AddBuffer(vb, vbl);

		return new mesh(va, ib);
	}

	iwmath::vector3 mesh::F(const float& u, const float& v, const float& r) {
		return iwmath::vector3(cos(u) * sin(v) * r, cos(v) * r, sin(u) * sin(v) * r);
	}
}