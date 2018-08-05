#pragma once
#include <string>

namespace graphics {
	struct shader_program_source;
	class shader_program {
	private:
		unsigned int m_render_id;

		shader_program_source read_shader(const char* file_name);
		unsigned int compile_shader(unsigned int type, std::string source);
	public:
		shader_program(const char* fileName);
		~shader_program();

		void use_program();
		void delete_program();
	};

	struct shader_program_source {
		std::string vertex;
		std::string fragment;
	};
}