#pragma once

#include <unordered_map>
#include "component_pool.h"

namespace iwecs {
	class ecs_manager {
	private:
		typedef std::unordered_map<std::size_t, icomponent_pool*> component_map;
		component_map m_components;
	public:
		ecs_manager();
		~ecs_manager();

		template<typename ComponentT, typename... ComponentArgsT>
		void add_component(unsigned int entity_id, ComponentArgsT&&... args);

		template<typename ComponentT>
		bool remove_component(unsigned int entity_id);
	};

	template<typename ComponentT, typename... ComponentArgsT>
	void ecs_manager::add_component(unsigned int entity_id, ComponentArgsT&&... args) {
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

	template<typename ComponentT>
	bool ecs_manager::remove_component(unsigned int entity_id) {
		typedef component_pool<ComponentT>* pool_ptr;

		std::size_t id = typeid(ComponentT).hash_code();
		pool_ptr pool;
		if (m_components.find(id) != m_components.end()) {
			pool = reinterpret_cast<pool_ptr>(m_components[id]);
			return pool->remove_component(entity_id);
		}

		return false;
	}
}