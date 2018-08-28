#pragma once

#include <vector>
#include "util/sparse_array.h"

namespace iwecs {
	template<typename... ComponentsT>
	class component_view {
	private:
		std::vector<sparse_array<ComponentsT>...> m_components;
	public:
		component_view(ComponentsT... components) {
			m_components = std::vector<ComponentsT...>{ components };
		}
	};
}