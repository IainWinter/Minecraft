#pragma once

namespace graphics {
	class index_buffer {
	private:
		unsigned int m_render_id;
		const void* m_data;
		unsigned int m_count;
	public:
		index_buffer(const void* data, unsigned int count);
		~index_buffer();

		void bind() const;
		void unbind() const;
		void draw() const;

		inline unsigned int count() const {
			return m_count;
		}
	};
}