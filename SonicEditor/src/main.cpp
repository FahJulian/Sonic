#include "sonic/debug/Log.h"

#include "sonic/core/scene/Scene.h"

#include "sonic/core/scene/serialization/SceneSerializer.h"

#include "sonic/components/ui/UIComponent.hpp"

int main()
{
	sonic::Log::init("C:dev/Sonic/Sandbox/log/test_log.log", &std::cout, sonic::Log::ALL, sonic::Log::ALL);
	sonic::client::initClientLog(sonic::Log::getInstance());

	sonic::Result<sonic::SceneFileData, sonic::SceneFileParsingError> sceneData = sonic::SceneSerializer::parse("C:/dev/Sonic/Sandbox/res/scenes/TestScene/TestScene.sonic");

	if (!sceneData.hasError)
		sonic::SceneSerializer::generateCode(sceneData.result);

	sonic::Optional<sonic::Scene*> scene = sonic::Scene::createScene("TestScene");

	if (scene.hasValue())
	{
		scene.getValue()->load(sceneData.result.data);
		scene.getValue()->init();

		scene.getValue()->addComponent<sonic::UIComponent>(0, sonic::UIComponent());
	}
}
