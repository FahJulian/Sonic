#pragma once
#include "Sonic/Base.h"
#include "Sonic/App.h"

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
    AppData init();
}

int main()
{
    Sonic::AppData data = Sonic::init();
    Sonic::App::init(data);

    Sonic::App::run();

    Sonic::App::destroy();
}
