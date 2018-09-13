#pragma once

#include <tuple>
#include "IwUtil/sparse_array.h"
#include "component_array.h"

template<typename... ComponentTs>
class component_view {
private:
	std::tuple<component_array<ComponentTs>...> m_components;
public:
	component_view();
	~component_view();
};