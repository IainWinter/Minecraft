#pragma once

#include <unordered_map>
#include "component_registry.h"

//Manager -> holds a component registry and systems and handles updates and events
//Component registry -> holds all components
//Component view -> templated type that holds pointers to the dense arrays of components of the templated type
//Component system -> Can be updated and given a component registry which can get specific views
//Component system (templated) -> Can be updated and given only the specific types of components


//Systems get sent iterators
//Manager uses registry to get iterators to the component arrays

class ecs_manager {
private:
	component_registry m_registry;
public:
	ecs_manager() {}
	~ecs_manager() {}

	component_registry& get_registry() {
		return m_registry;
	}

	template<typename ComponentT, typename... ComponentArgsT>
	void add_component(unsigned int entity, ComponentArgsT... args);
};

template<typename ComponentT, typename ...ComponentArgsT>
void ecs_manager::add_component(unsigned int entity, ComponentArgsT... args) {
	ComponentT c = ComponentT(args...);
	m_registry.add_component<ComponentT>(entity, c);
}