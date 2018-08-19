#pragma once

#include <unordered_map>
#include "util/sparse_array.h"
#include <vector>

namespace iwecs {
	class icomponent_pool {
	public:
		virtual ~icomponent_pool() {}
	};

	template<typename ComponentT>
	class component_pool : public icomponent_pool {
	private:
		sparse_array<ComponentT> m_components;
	public:
		component_pool() 
			: m_components(10) {}

		~component_pool() {}

		void add_component(unsigned int entity_id, const ComponentT& component) {
			m_components.insert(entity_id, component);
		}

		void remove_component(unsigned int entity_id) {
			m_components.remove(entity_id);
		}

		ComponentT* get_component(unsigned int entity_id) {
			return m_components.at(entity_id);
		}
	};

	struct transform {
		float x, y, z;

		transform() {}

		transform(float x, float y, float z)
			: x(x), y(y), z(z)
		{}
	};

	class ecs_manager {
	private:
		std::unordered_map<unsigned int, icomponent_pool*> m_components;
	public:
		ecs_manager() {
			component_pool<transform>* s = new component_pool<transform>();
			m_components[0] = s;
			((component_pool<transform>*)m_components[0])->add_component(0, transform(1, 2, 3));
		}
	};
}