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
	using iterator = typename iwutil::sparse_array<ComponentT>::iterator;

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

	iterator begin() {
		return m_components.begin();
	}

	iterator end() {
		return m_components.end();
	}
};