#pragma once

#include <string>
#include <vector>

namespace io {
	std::string read_file(const char* filePath);
	std::vector<std::string> read_file_lines(const char* filePath);
	bool file_exists(const char* filePath);
	uintmax_t get_file_size(const char* filePath);
}