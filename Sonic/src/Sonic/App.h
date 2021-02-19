#pragma once
#include "Base.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Graphics/Color.h"
#include "Sonic/Event/Events/WindowEvents.h"
#include <string>

extern int main();

namespace Sonic {

    struct AppData
    {
        WindowMode windowMode;
        String windowTitle;
        Color clearColor;
        bool windowResizable;
        int windowWidth;
        int windowHeight;
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
