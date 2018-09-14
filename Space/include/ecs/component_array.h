#pragma once

#include "IwUtil/sparse_array.h"

class icomponent_array {
public:
	virtual ~icomponent_array() {}
};

template<typename ComponentT>
class component_array : public icomponent_array {
private:
	iwutil::sparse_array<ComponentT> m_components;
public:
	component_array() 
		: m_components(10) {}

	~component_array() {}

	void add_component(int entity, const ComponentT& component) {
		m_components.insert(entity, component);
	}

	void remove_component(int entity) {
		m_components.remove(entity);
	}

	ComponentT get_component(int entity) {
		return m_components.at(entity);
	}

	std::pair<unsigned int, ComponentT*> get_components() {
		return std::make_pair(m_components.count(), m_components.data());
	}
};