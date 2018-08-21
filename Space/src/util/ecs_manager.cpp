#include"ecs/ecs_manager.h"

namespace iwecs {
	ecs_manager::ecs_manager() {

	}

	ecs_manager::~ecs_manager() {
		for (component_map::iterator itr = m_components.begin(); itr != m_components.end(); itr++) {
			delete (*itr).second;
		}
	}
}