#pragma once
#include "GL/glew.h"
#include <iostream>
#include "Window/Window.h"
#include "Scene/Scene.h"

extern int main();

namespace Sonic {

    class App
    {
    protected:
        App(int width, int height, const char* title);

        virtual Scene* OnInit() = 0;
        virtual void OnExit() {}
        virtual ~App();
        
    public:
        static App* get() { return m_Instance; }

        void Stop();
        
    private:
        bool Init();
        void Run();
        void Exit();

        void OnWindowClosed(const WindowClosedEvent& event);

    private:
        bool m_Running;
        Scene* m_Scene;

        static App* m_Instance;

        friend int ::main();
    };

}
