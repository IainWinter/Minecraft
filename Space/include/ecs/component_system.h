#pragma once

namespace iwecs {
	class icomponent_system {
	public:
		virtual ~icomponent_system() {}
	};

	template<typename... ComponentTs>
	class component_system : public icomponent_system {
	public:
		virtual ~component_system() {}

		virtual void process(ComponentTs&... components) = 0;
	};
}