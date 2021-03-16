#pragma once
#include <json/json.hpp>
#include "Sonic/Base.h"
#include "Sonic/Scene/Scene.h"

namespace Sonic::Serialization {

	void deserializeEntities(Scene* scene, const String& relativeFilePath, std::map<String, Entity>& entities, const JSON& data);

}
