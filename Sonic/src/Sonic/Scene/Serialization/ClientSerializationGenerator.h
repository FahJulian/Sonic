#pragma once
#include "Sonic/Base.h"

namespace Sonic::Serialization {

	class ClientSerializationGenerator
	{
		static void generateClientSerializationCode(const std::vector<String>& scripts, const String& relativeSceneFilePath, bool releaseBuild);

		friend class SceneSerializer;
	};

}