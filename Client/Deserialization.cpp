#include "sonic/core/scene/serialization/ClientSerialization.h"

#include "TestScene.hpp"

namespace sonic::client
{
	Scene* createScene(String sceneName)
	{
		if (sceneName == "TestScene")
			return new TestScene();

		return nullptr;
	}
}
