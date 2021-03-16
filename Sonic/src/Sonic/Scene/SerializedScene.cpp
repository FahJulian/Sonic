#include "Serialization/SceneSerializer.h"
#include "SerializedScene.h"

using namespace Sonic;


void SerializedScene::OnLoad()
{
	Serialization::SceneSerializer::deserialize(this, m_RelativeFilePath, false);
}

void SerializedScene::OnSave()
{
	Serialization::SceneSerializer::serialize(this, m_RelativeFilePath);
}
