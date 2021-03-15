#include <Sonic/main.h>

using namespace Sonic;


String Sonic::init()
{
	SceneManager::registerScene(new SerializedScene("scenes/sandbox"), "sandbox");

	return "windows/sandbox";
}

const String& Sonic::resourceDir()
{
	static String dir = "C:/dev/Sonic/Sandbox/res/";
	return dir;
}
