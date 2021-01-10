#pragma once
#include <gl/glew.h>
#include "glfw/glfw3.h"
#include "Scene/Scene.h"

extern int main();

namespace Sonic {

    class App
    {
    protected:
        App(int width, int height, const char* title, bool windoResizable = true);

        virtual Scene* OnInit() = 0;
        virtual void OnExit() {}
        virtual ~App();
        
    public:
        static App* get() { return m_Instance; }

        void Stop();

        Scene* GetActiveScene() { return m_Scene; }
        
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
