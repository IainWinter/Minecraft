#include "graphics\vertex_buffer_layout.h"

namespace graphics {
	unsigned int vertex_buffer_layout_element::sizeof_type(unsigned int gl_type) {
		switch (gl_type) {
			case GL_FLOAT: return sizeof(GLfloat);
			case GL_UNSIGNED_INT: return sizeof(GLuint);
			case GL_UNSIGNED_BYTE: return sizeof(GLbyte);
		}

		return 0;
	}

}