#pragma once

#include "Entity.h"

namespace sonic
{
	class Scene
	{
	public:
		virtual void init() = 0;
		virtual void update(float deltaTime) = 0;
	};

} // namespace sonic
