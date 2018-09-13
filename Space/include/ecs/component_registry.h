#pragma once

#include <unordered_map>
#include "component_array.h"

class component_registry {
private:
	std::unordered_map<std::size_t, icomponent_array> m_components;
public:
	component_registry();
	~component_registry();

	template<typename ComponentT>
	void add_component(unsigned int entity, const ComponentT& component);
	void remove_component(unsigned int entitiy);
};