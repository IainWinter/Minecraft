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
		component_pool(unsigned int default_count) 
			: m_components(default_count) {}

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
		typedef std::unordered_map<std::size_t, icomponent_pool*> component_map;
		component_map m_components;
	public:
		ecs_manager() {

		}

		~ecs_manager() {
			for (component_map::iterator itr = m_components.begin(); itr != m_components.end(); itr++) {
				delete (*itr).second;
			}
		}

		template<typename ComponentT, typename... ComponentArgsT>
		void add_component(unsigned int entity_id, ComponentArgsT&&... args) {
			typedef component_pool<ComponentT>* pool_ptr;

			std::size_t id = typeid(ComponentT).hash_code();

			pool_ptr pool;
			if (m_components.find(id) == m_components.end()) {
				pool = new component_pool<ComponentT>(10); //Default size
				m_components[id] = pool;
			} else {
				pool = reinterpret_cast<pool_ptr>(m_components[id]);
			}

			pool->add_component(entity_id, ComponentT(args...));
		}

		//Remove component
		template<typename ComponentT>
		bool remove_component(unsigned int entity_id) {
			typedef component_pool<ComponentT>* pool_ptr;

			std::size_t id = typeid(ComponentT).hash_code();

			pool_ptr pool;
			if (m_components.find(id) != m_components.end()) {
				pool = reinterpret_cast<pool_ptr>(m_components[id]);
				return pool->remove_component(entity_id);
			}

			return false;
		}

		//Update
	};
}