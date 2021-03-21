#include "sonic/core/scene/serialization/ClientSerialization.h"
#include "sonic/core/scene/serialization/SceneSerializer.h"
#include "sonic/debug/Log.h"

int main()
{
	sonic::Log::init("C:dev/Sonic/Sandbox/log/test_log.log", &std::cout, sonic::Log::ALL, sonic::Log::ALL);

	sonic::Result<sonic::SceneData, sonic::SceneFileParsingError> result = sonic::SceneSerializer::parse("C:/dev/Sonic/Sandbox/TestScene.sonic");

}
