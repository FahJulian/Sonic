#pragma once
#include "Sonic/App.h"
#include "Sonic/Scene/Scene.h"

namespace Sonic
{
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
