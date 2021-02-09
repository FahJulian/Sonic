#pragma once
#include <cstdint>

namespace Sonic {

	using ComponentType = uint16_t;

	ComponentType registerComponent(size_t componentSize);

	size_t getComponentSize(ComponentType type);

	template<typename Component>
	ComponentType getComponentType()
	{
		static ComponentType type = registerComponent(sizeof(Component));
		return type;
	}

}