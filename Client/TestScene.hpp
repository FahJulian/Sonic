#include <iostream>

#include "sonic/core/scene/serialization/ClientSerialization.h"

namespace sonic::client
{
	class TestScene : public Scene
	{
		void init() override
		{
			std::cout << "TestScene initialized" << std::endl;
		}

		void update(float deltaTime) override
		{
			std::cout << "TestScene updated" << std::endl;
		}
	};
}
