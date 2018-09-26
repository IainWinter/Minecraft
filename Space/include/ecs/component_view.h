#pragma once

#include <tuple>
#include "component_array.h"

template<typename... ComponentsT>
class component_view {
private:
	using component_tuple = typename std::tuple<ComponentsT*...>;
	using component_array_tuple = typename std::tuple<typename component_array<ComponentsT>::iterator...>;

	component_array_tuple m_components;
	unsigned int m_count;
public:
	component_view(component_array<ComponentsT>*... component_arrays)
		: m_components(std::make_tuple<typename component_array<ComponentsT>::iterator...>(
			component_arrays->begin()...))
	{
		m_count = 0;
	}

	~component_view() {}

	//component_tuple get(unsigned int index) {
	//	return std::make_tuple(*std::get<0>(m_components), *std::get<1>(m_components));
	//}
};