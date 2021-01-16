#pragma once
#include <gl/glew.h>
#include "glfw/glfw3.h"
#include "Scene/Scene.h"

extern int main();

namespace Sonic {

    /**
    * Class that should be subclassed by the App that is created. Holds the 
    * currently active scene of the app.
    */
    class App
    {
    protected:
        /**
        * Creates a new App
        * 
        * @param width The width of the window of the app
        * @param height The height of the window of the app
        * @param title The title of the window of the app
        * @param windowResizable Whether or not the window of the app should be 
        *                        resizable by the user
        */
        App(int width, int height, const char* title, bool windowResizable = true);

        /**
        * Virtual method that must be overriden by the subclass. It is called when
        * the app is initialized and should return a new instance of the scene
        * that should be active when the application starts
        */
        virtual Scene* OnInit() = 0;

        /**
        * Virtual method that can be override by the subclass. It is called when the 
        * app exits.
        */
        virtual void OnExit() {}

        /**
        * Virtual destructor that can be overriden by the subclass.
        */
        virtual ~App();
        
    public:
        /**
        * @return A pointer to the currently active app
        */
        static App* get() { return m_Instance; }

        /**
        * Stops the app after the currently acitve update / render phase
        */
        void Stop();

        /**
        * @return A pointer to the currently active scene
        */
        Scene* GetActiveScene() { return m_Scene; }

        void SetScene(Scene* scene) { m_Scene = scene; scene->Init(); }
        
    private:
        /**
        * Initializes the app. 
        * 
        * @return Whether or not initialization was successful
        */
        bool Init();

        /**
        * Runs the app.
        */
        void Run();

        /**
        * Exits the app.
        */
        void Exit();

        void OnWindowClosed(const WindowClosedEvent& event);

    private:
        bool m_Running;
        Scene* m_Scene;

        static App* m_Instance;

        friend int ::main();
    };

}
