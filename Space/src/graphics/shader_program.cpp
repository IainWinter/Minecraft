#include <sstream>
#include <vector>
#include "graphics\shader_program.h"
#include "GL\glew.h"
#include "io\file.h"

namespace graphics {
	shader_program::shader_program(const char* fileName) {
		shader_program_source source = read_shader(fileName);

		m_render_id = glCreateProgram();
		unsigned int vsid = compile_shader(GL_VERTEX_SHADER, source.vertex);
		unsigned int fsid = compile_shader(GL_FRAGMENT_SHADER, source.fragment);

		glAttachShader(m_render_id, vsid);
		glAttachShader(m_render_id, fsid);
		glLinkProgram(m_render_id);
		glValidateProgram(m_render_id);

		glDeleteShader(vsid);
		glDeleteShader(fsid);
	}

	shader_program::~shader_program() {
		delete_program();
	}

	shader_program_source shader_program::read_shader(const char* fileName) {
		std::vector<std::string> code = io::read_file_lines(fileName);
		std::stringstream ss[2];
		enum ShaderMode {
			NONE = -1,
			VERTEX,
			FRAGMENT
		};

		ShaderMode mode;
		for (std::string& line : code) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos)  mode = VERTEX;
				else if (line.find("fragment") != std::string::npos)  mode = FRAGMENT;
			} else {
				ss[mode] << line << std::endl;
			}
		}

		return shader_program_source{ ss[VERTEX].str(), ss[FRAGMENT].str() };
	}

	unsigned int shader_program::compile_shader(unsigned int type, std::string source) {
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (!result) {
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

			char* message = (char*)_malloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);

			glDeleteShader(id);

			return 0;
		}

		return id;
	}

	void shader_program::use_program() {
		glUseProgram(m_render_id);
	}

	void shader_program::delete_program() {
		glDeleteProgram(m_render_id);
	}
}