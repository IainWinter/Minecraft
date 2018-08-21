#pragma once

#include "util/sparse_array.h"

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
		component_pool(unsigned int default_count);
		~component_pool();

		void add_component(unsigned int entity_id, const ComponentT& component);
		void remove_component(unsigned int entity_id);
		ComponentT* get_component(unsigned int entity_id);
	};

	template<typename ComponentT>
	component_pool<ComponentT>::component_pool(unsigned int default_count)
		: m_components(default_count) {
	}

	template<typename ComponentT>
	component_pool<ComponentT>::~component_pool() {}

	template<typename ComponentT>
	void component_pool<ComponentT>::add_component(unsigned int entity_id, const ComponentT& component) {
		m_components.insert(entity_id, component);
	}

	template<typename ComponentT>
	void component_pool<ComponentT>::remove_component(unsigned int entity_id) {
		m_components.remove(entity_id);
	}

	template<typename ComponentT>
	ComponentT* component_pool<ComponentT>::get_component(unsigned int entity_id) {
		return m_components.at(entity_id);
	}
}