#include "SceneSerializer.h"
#include "SerializedScene.h"

using namespace Sonic;


void SerializedScene::OnLoad()
{
	SceneSerializer::deserialize(this, m_RelativeFilePath);
}

void SerializedScene::OnSave()
{
	SceneSerializer::serialize(this, m_RelativeFilePath);
}
