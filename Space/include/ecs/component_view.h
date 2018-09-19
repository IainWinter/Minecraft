#pragma once

#include <tuple>
#include "component_array.h"

template<typename... ComponentsT>
class component_view {
private:
	using component_tuple = typename std::tuple<typename component_array<ComponentsT>::iterator...>;

	component_tuple m_components;
	unsigned int m_count;
public:
	component_view(typename component_array<ComponentsT>::iterator... component_itrs) {
		m_components = std::make_tuple<typename component_array<ComponentsT>::iterator...>(
			component_itrs...
			);
	}

	~component_view() {}


};