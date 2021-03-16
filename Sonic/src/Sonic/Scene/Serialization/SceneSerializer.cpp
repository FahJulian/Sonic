#include <json/json.hpp>
#include "Sonic/App.h"
#include "Sonic/Util/StringUtils.h"
#include "ClientSerializationGenerator.h"
#include "EntityDeserialization.h"
#include "SceneSerializer.h"

using namespace Sonic::Serialization;


void SceneSerializer::deserialize(Scene* scene, const String& relativeFilePath, bool recompile)
{
	String sceneName = relativeFilePath.substr(relativeFilePath.find_last_of('/') + 1);
	InputFileStream file = InputFileStream(resourceDir() + relativeFilePath + "/" + sceneName + ".sonicscene.json");

	JSON json;

	try 
	{
		file >> json;
	}
	catch (nlohmann::detail::parse_error)
	{
		SONIC_LOG_WARN("Could not deserialize scene ", relativeFilePath, ": Could not extract JSON from file");
		return;
	}

	if (recompile)
	{
		std::vector<String> scriptFiles;
		for (auto& scriptFile : json["scripts"])
			scriptFiles.push_back(resourceDir() + relativeFilePath + "/" + scriptFile.get<String>());

		ClientSerializationGenerator::generateClientSerializationCode(scriptFiles, relativeFilePath, false);
	}

	std::map<String, Entity> entityIDs;
	deserializeEntities(scene, relativeFilePath, entityIDs, json);
}

void SceneSerializer::serialize(Scene* scene, const String& relativeFilePath)
{

}
