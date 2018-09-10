#pragma once

#include <unordered_map>
#include "component_pool.h"
#include "component_system.h"

namespace iwecs {
	class ecs_manager {
	private:
		typedef std::pair<unsigned char, const std::size_t*> component_id_pair;
		typedef std::pair<component_id_pair, icomponent_system*> system_pair;
		typedef std::unordered_map<std::size_t, icomponent_pool*> component_map;
		typedef std::vector<system_pair> system_vector;
		component_map m_components;
		system_vector m_systems;
	public:
		ecs_manager();
		~ecs_manager();

		void update();

		template<typename ComponentT, typename... ComponentArgsT>
		void add_component(unsigned int entity_id, ComponentArgsT&&... args);

		template<typename ComponentT>
		bool remove_component(unsigned int entity_id);

		template<typename SystemT, typename... ComponentTs>
		void add_system();
	};

	template<typename ComponentT, typename... ComponentArgsT>
	void ecs_manager::add_component(unsigned int entity_id, ComponentArgsT&&... args) {
		typedef component_pool<ComponentT>* pool_ptr;

		pool_ptr pool;
		std::size_t id = typeid(ComponentT).hash_code();
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

		pool_ptr pool;
		std::size_t id = typeid(ComponentT).hash_code();
		if (m_components.find(id) != m_components.end()) {
			pool = reinterpret_cast<pool_ptr>(m_components[id]);
			return pool->remove_component(entity_id);
		}

		return false;
	}

	template<typename SystemT, typename ...ComponentTs>
	void ecs_manager::add_system() {
		std::initializer_list<std::size_t> ids = {
			typeid(ComponentTs).hash_code()...
		};

		unsigned char count = ids.size();
		std::size_t* component_ids = new std::size_t[count];
		for (std::size_t i = 1; i < count; i++) {
			component_ids[i] = *(ids.begin() + i);
		}

		component_id_pair id_pair = std::make_pair(count, component_ids);
		system_pair pair = std::make_pair(id_pair, new SystemT());
		m_systems.push_back(pair);
	}
}
