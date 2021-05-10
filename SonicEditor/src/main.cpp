#include <iostream>

#include "sonic/debug/Log.h"

#include "sonic/core/scene/Scene.h"

#include "sonic/core/scene/serialization/SceneSerializer.h"

#include "sonic/components/ui/UIComponent.hpp"

#include "sonic/util/Function.h"

//#include "sonic/util/rcr/DllReloader.h"

#include <functional>

void func1(int a)
{
	sonic::Log::trace(a);
}

namespace sonic::client
{
	String getClientDllPath()
	{
		return "C:\\dev\\Sonic\\Sandbox\\sandbox.dll";
	}
}

int main()
{
	sonic::Log::init("C:dev/Sonic/Sandbox/log/test_log.log", std::cout, sonic::Log::ALL, sonic::Log::ALL);

	sonic::Result<sonic::SceneFileData, sonic::SceneFileParsingError> sceneData = sonic::SceneSerializer::parse("C:/dev/Sonic/Sandbox/res/scenes/TestScene/TestScene.sonic");

	if (!sceneData.hasError)
		sonic::SceneSerializer::generateCode(sceneData.result);

	sonic::DllReloader::Settings dllReloaderSettings;
	dllReloaderSettings.compilerExePath = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.28.29910\\bin\\HostX86\\x64\\cl.exe";
	dllReloaderSettings.linkerExePath = "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.28.29910\\bin\\HostX86\\x64\\link.exe";
	dllReloaderSettings.tmpDir = "C:\\Users\\fahju\\AppData\\Roaming\\Sonic\\tmp\\";
	dllReloaderSettings.includeDirs = 
	{ 
		"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.28.29910\\include",
		"C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.19041.0\\ucrt",
		"C:\\dev\\Sonic\\Sonic\\src",
	};

	dllReloaderSettings.libDirs = 
	{ 
		"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.28.29910\\lib\\x64\\uwp",
		"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.19041.0\\um\\x86",
	};

	dllReloaderSettings.configuration = sonic::DllReloader::Configuration::DEBUG;

	sonic::DllReloader::init(dllReloaderSettings);

	sonic::DllReloader::reloadDll("C:\\dev\\Sonic\\Sandbox\\sandbox.dll", true, 
		{
			"C:\\Users\\fahju\\AppData\\Roaming\\Sonic\\tmp\\sandbox\\sandbox.cpp",
		}, 
		{
			"SN_CLIENT_DYNAMIC" 
		}
	);

	sonic::client::initClientLog(sonic::Log::getInstance());

	sonic::Optional<sonic::client::Scene*> scene = sonic::client::createScene("TestScene");

	if (scene.hasValue())
	{
		scene.getValue()->load(sceneData.result.data);
		scene.getValue()->init();

		scene.getValue()->addComponent<sonic::UIComponent>(0, sonic::UIComponent());
	}
}
