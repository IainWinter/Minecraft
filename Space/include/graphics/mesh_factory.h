#pragma once
#include "mesh.h"
#include <unordered_map>
#include "util/pair_hash.h"

namespace graphics {
	class mesh_factory {
	private:
		typedef std::pair<unsigned int, unsigned int> index_pair;
		typedef std::unordered_map<index_pair, unsigned int, pair_hash> index_lookup_map;
		typedef std::vector<iwmath::vector3> vert_list;

		static const unsigned int ico_vert_count;
		static const unsigned int ico_index_count;
		static const iwmath::vector3* ico_verts;
		static const unsigned int* ico_index;

		static void sub_devide(
			iwmath::vector3* verts, 
			unsigned int* index, 
			unsigned int& current_index_count, 
			unsigned int& current_vert_count);

		static unsigned int create_vertex_for_edge(
			index_lookup_map& lookup, 
			iwmath::vector3* verts, 
			unsigned int first, 
			unsigned int second, 
			unsigned int& current_vert_count);
	public:
		static mesh* create_sphere(const unsigned int& resolution);
	};
}