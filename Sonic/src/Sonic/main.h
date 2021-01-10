#pragma once
#include "Sonic/App.h"
#include "Sonic/Scene/Scene.h"

/*
* This header file should be included by the main file of the
* created application. The application then has to define 
* Sonic::createApplication() which should return a pointer
* to the instance of the subclassed App that should be ran
*/

namespace Sonic
{
    /**
    * Function that has to be defined by the client which should 
    * return a pointer to the instance of the subclassed App that 
    * should be ran
    */
    App* createApplication();
}

int main()
{
    Sonic::App* app = Sonic::createApplication();
    app->Init();

    app->Run();

    app->Exit();
    delete app;
}
