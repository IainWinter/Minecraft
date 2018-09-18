#pragma once

#include <tuple>

template<typename... ComponentsT>
class component_view {
private:
	std::tuple<ComponentsT*...> m_components;
	unsigned int m_count;
public:
	component_view(component_array<ComponentsT>*... components) {
		m_components = std::make_tuple<ComponentsT*...>(components->get_components().second...);
		
	}

	~component_view() {}

	//std::tuple<ComponentTs&...> operator[](unsigned int index);
};

//template<typename... ComponentTs>
//std::tuple<ComponentTs&...> component_view<...ComponentTs>::operator[](unsigned int index) {
//	
//}
