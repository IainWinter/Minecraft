#pragma once

#include <tuple>
#include "component_array.h"

template<typename... ComponentsT>
class component_view {
private:
	std::tuple<component_array<ComponentsT>...> m_components;
	unsigned int m_count;
public:
	component_view(component_array<ComponentsT>... components) {
		m_components = std::make_tuple<ComponentsT*...>(components->get_components()); 



		component_array<int> d = component_array<int>();
		d.add_component(0, 1);
		d.add_component(4, 123);
		component_array<int>::iterator itr = d.begin();
		int i = *itr;
		itr++;
		int ii = *itr;
		bool isEnd = itr == d.end();
		itr++;
		int iii = *itr;
	}

	~component_view() {}

	//std::tuple<ComponentTs&...> operator[](unsigned int index);
};

//template<typename... ComponentTs>
//std::tuple<ComponentTs&...> component_view<...ComponentTs>::operator[](unsigned int index) {
//	
//}
