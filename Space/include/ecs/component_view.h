#pragma once

#include <tuple>
#include "component_array.h"

template<typename... ComponentsT>
class component_view {
private:
	struct ssss {
		std::tuple<component_array<ComponentsT>::iterator...> components;
		unsigned int m_count;
	};
public:
	component_view(component_array<ComponentsT>*... components) {
		component_array<ComponentsT>::iterator


		ccomponents = std::make_tuple<component_array<ComponentsT>::iterator...>(
			components->begin()...
		);
	}

	~component_view() {}


};