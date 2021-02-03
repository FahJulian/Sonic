#pragma once
#include <string>

extern int main();

namespace Sonic {

    class Scene;

    struct WindowClosedEvent;

    struct WindowResizedEvent;

    struct AppData
    {
        int windowWidth;
        int windowHeight;
        std::string windowTitle;
        bool windowResizable;
        Scene* scene;
    };

    /**
    * Class that should be subclassed by the App that is created. Holds the 
    * currently active scene of the app.
    */
    class App
    {
    public:
        static void setUpdateCap(double newCap);

        /**
        * Stops the app after the currently acitve update / render phase
        */
        static void stop();

        /**
        * @return A pointer to the currently active scene
        */
        static Scene* getActiveScene();

        static void setScene(Scene* scene);
        
    private:
        /**
        * Initializes the app. 
        * 
        * @return Whether or not initialization was successful
        */
        static bool init(const AppData& data);

        /**
        * Runs the app.
        */
        static void run();

        static void onWindowClosed(const WindowClosedEvent& e);

        static void onWindowResized(const WindowResizedEvent& e);

        static void destroy();

        friend int ::main();
        friend class Window;
    };

}
