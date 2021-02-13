#include <GL/glew.h>
#include <string>
#include "Sonic/Base.h"
#include "Scene/Scene.h"
#include "Sonic/Graphics/Font.h"
#include "Sonic/Renderer/Font/FontRenderer.h"
#include "Sonic/Renderer/2D/Renderer2D.h"
#include "Sonic/Renderer/UI/UIRenderer.h"
#include "Sonic/Scene/SceneManager.h"
#include "Window/Window.h"
#include "App.h"

using namespace Sonic;


static const int INITIAL_UPDATE_CAP = 60;
static double s_SecondsPerUpdate = 1.0 / INITIAL_UPDATE_CAP;

static bool s_Running = false;


bool App::init(const AppData& data)
{
    if (!Sonic::Window::init(data.windowWidth, data.windowHeight,
        data.windowTitle, data.windowResizable))
    {
        return false;
    }

    if (glewInit() != GLEW_OK)
    {
        Window::destroy();
        return false;
    }

    Renderer2D::init();
    UIRenderer::init();
    Font::init();
    FontRenderer::init();
    SceneManager::init();
        
    return true;
}

void App::run()
{
    s_Running = true;

    double totalDelta = 0.0;
    double startTime = Window::getTime();

#ifdef SONIC_DEBUG
    double fpsTimer = 0.0f;
    int frames = 0;
    int lastFpsLength = 0;
#endif

    while (s_Running)
    {
        if (totalDelta >= s_SecondsPerUpdate)
        {
            SceneManager::s_CurrentScene->Update(static_cast<float>(totalDelta));
            SceneManager::s_CurrentScene->RebufferRenderers();
            Window::pollEvents();

            totalDelta = 0;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        SceneManager::getCurrentScene()->Render();
        Window::swapBuffers();

        const double endTime = Window::getTime();
        const double delta = endTime - startTime;
            
#ifdef SONIC_DEBUG
        frames++;
        fpsTimer += delta;
        if (fpsTimer >= 1.0f)
        {
            fpsTimer -= 1.0f;
            String oldTitle = String(Window::getTitle());
            String framesStr = std::to_string(frames);
            String sub = oldTitle.substr(0, oldTitle.length() - lastFpsLength);
            Window::setTitle(oldTitle.substr(0, oldTitle.length() - lastFpsLength) + " (" + framesStr + ")");
            lastFpsLength = static_cast<int>(framesStr.length()) + 3;
            frames = 0;
        }
#endif

        totalDelta += endTime - startTime;
        startTime = endTime;
    }
}

void App::setUpdateCap(double newCap)
{
    s_SecondsPerUpdate = 1.0 / newCap;
}

void App::stop()
{
    s_Running = false;
}

void App::onWindowResized(const WindowResizedEvent& e)
{
    SceneManager::getCurrentScene()->RebufferRenderers();

    Window::clear();
    SceneManager::getCurrentScene()->Render();
    Window::swapBuffers();
}

void App::onWindowClosed(const WindowClosedEvent& e)
{
    App::stop();
}

void App::destroy()
{ 
    Renderer2D::destroy();
    FontRenderer::destroy();
    UIRenderer::destroy();

    SceneManager::destroy();
    Font::destroy();
    Window::destroy();
}
