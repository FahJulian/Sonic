#include <Sonic/main.h>

#include "Sonic/Util/Callable.h"
#include "Sonic/Scene/Serialization/CallableSignature.h"

using namespace Sonic;


struct Test
{
	void Func1()
	{

	}

	int Func2(double a)
	{
		return 0;
	}
};

void func1()
{

}

double func2(int a)
{
	return 0.0;
}


String Sonic::init()
{
	Test t = { };
	Method<Test, void()>* c1 = new Method<Test, void()>(&t, &Test::Func1);
	Ref<Callable<int(double)>> c2 = Ref<Callable<int(double)>>(new Method<Test, int(double)>(&t, &Test::Func2));

	Ref<Callable<void()>> c3 = Ref<Callable<void()>>(new Function<void()>(func1));
	Function<double(int)>* c4 = new Function<double(int)>(func2);

	bool b1 = CallableSignature(c1) == &Test::Func1;
	bool b2 = CallableSignature(c2) == &Test::Func2;
	bool b3 = CallableSignature(c3) == func1;
	bool b4 = CallableSignature(c4) == func2;

	bool b5 = CallableSignature(c1) == &Test::Func2;
	bool b6 = CallableSignature(c1) == func1;
	bool b7 = CallableSignature(c1) == func2;

	bool b8 = CallableSignature(func1) == &Test::Func1;

	SceneManager::registerScene(new SerializedScene("scenes/sandbox"), "sandbox");

	SceneManager::loadScene("sandbox");
	std::cin.get();

	return "windows/sandbox";
}

const String& Sonic::resourceDir()
{
	static String dir = "C:/dev/Sonic/Sandbox/res/";
	return dir;
}
