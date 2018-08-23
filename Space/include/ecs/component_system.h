#pragma once

namespace iwecs {
	class icomponent_system {
	public:
		virtual ~icomponent_system() {}
	};

	template<typename... ComponentTs>
	class component_system : public icomponent_system {
	protected:
		typedef std::tuple<std::pair<unsigned int, ComponentTs*>...> component_pack;
	public:
		virtual ~component_system() {}

		virtual void process(component_pack& components) = 0;
	};
}