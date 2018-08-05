#pragma once

namespace graphics {
	class vertex_buffer {
	private:
		unsigned int m_render_id;
		const void* m_data;
	public:
		vertex_buffer(const void* data, unsigned int size);
		~vertex_buffer();

		void bind() const;
		void unbind() const;
	};
}