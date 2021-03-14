#pragma once
#include "Sonic/Base.h"
#include "Sonic/App.h"
#include "Sonic/Scene/SceneManager.h"
#include "Sonic/Scene/Serialization/SerializedScene.h"

/*
* This header file should be included by the main file of the
* created application. The application then has to define 
* Sonic::createApplication() which should return a pointer
* to the instance of the subclassed App that should be ran
*/

namespace Sonic
{
    String init();

    const String& resourceDir();
}

int main()
{
    Sonic::App::init(Sonic::init());

    Sonic::App::run();

    Sonic::App::destroy();
}
