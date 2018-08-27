#include"ecs/ecs_manager.h"

namespace iwecs {
	ecs_manager::ecs_manager() {}

	ecs_manager::~ecs_manager() {
		for (component_map::iterator itr = m_components.begin(); itr != m_components.end(); itr++) {
			delete (*itr).second;
		}

		for (system_vector::iterator itr = m_systems.begin(); itr != m_systems.end(); itr++) {
			delete (*itr).first.second;
			delete (*itr).second;
		}
	}

	void ecs_manager::update() {
		for (auto pair : m_systems) {
			component_id_pair& component_pair = pair.first;
			auto& system = pair.second;
			//system.process();
			

			unsigned char count = pair.first.first;
			const std::size_t* component_ids = pair.first.second;

			//Loop through the ids and get the data from the component pools from the map
		}
	}
}