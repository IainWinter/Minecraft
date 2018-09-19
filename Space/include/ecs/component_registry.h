#pragma once

#include <unordered_map>
#include "component_array.h"
#include "component_view.h"

class component_registry {
private:
	typedef std::unordered_map<std::size_t, icomponent_array*> component_map;
	component_map m_components;	

	template<typename ComponentT>
	component_array<ComponentT>* get_component_array();
public:
	component_registry() {}
	~component_registry() {
		for (component_map::iterator itr = m_components.begin(); itr != m_components.end(); itr++) {
			delete (*itr).second;
		}
	}

	template<typename ComponentT>
	void add_component(unsigned int entity, const ComponentT& component);

	template<typename ComponentT>
	bool remove_component(unsigned int entitiy);

	template<typename... ComponentsT>
	component_view<ComponentsT...> view_components();

	//view all components of a specified entity by id
};

template<typename ComponentT>
component_array<ComponentT>* component_registry::get_component_array() {
	typedef component_array<ComponentT>* array_ptr;

	std::size_t id = typeid(ComponentT).hash_code();
	if (m_components.find(id) != m_components.end()) {
		return reinterpret_cast<array_ptr>(m_components[id]);
	}

	return nullptr;
}

template<typename ComponentT>
void component_registry::add_component(unsigned int entity, const ComponentT& component) {
	typedef component_array<ComponentT>* array_ptr;

	array_ptr c_array;
	std::size_t id = typeid(ComponentT).hash_code();
	if (m_components.find(id) == m_components.end()) {
		c_array = new component_array<ComponentT>(); //Make a new component_array
		m_components[id] = c_array;
	} else {
		c_array = reinterpret_cast<array_ptr>(m_components[id]);
	}

	c_array->add_component(entity, component);
}

template<typename ComponentT>
bool component_registry::remove_component(unsigned int entitiy) {
	typedef component_array<ComponentT>* array_ptr;

	array_ptr c_array;
	std::size_t id = typeid(ComponentT).hash_code();
	if (m_components.find(id) != m_components.end()) {
		c_array = reinterpret_cast<array_ptr>(m_components[id]);
		return c_array->remove_component(entity_id);
	}

	return false;
}

template<typename... ComponentsT>
component_view<ComponentsT...> component_registry::view_components() {
	return component_view<ComponentsT...>(
		get_component_array<ComponentsT>()...
	);
}
