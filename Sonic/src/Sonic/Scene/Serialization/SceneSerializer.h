#pragma once
#include "Sonic/Base.h"
#include "Sonic/Scene/Scene.h"

namespace Sonic::Serialization {

	class SceneSerializer
	{
	public:
		static void deserialize(Scene* scene, const String& relativeFilePath, bool recompile);
		static void serialize(Scene* scene, const String& filePath);
	};

}
