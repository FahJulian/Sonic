#include <GL/glew.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <string>
#include "App.h"
#include "Renderer/Renderer2D.h"
#include "Window/Window.h"

struct InitialWindowData
{
    int width;
    int height;
    const char* title;
    bool resizable;
};

static InitialWindowData s_InitialWindowData;

namespace Sonic {
    
    App* App::m_Instance;

    App::App(int width, int height, const char* title, bool windowResizable)
        : m_Scene(nullptr), m_Running(false)
    {
        if (m_Instance != nullptr)
            return;
       
        App::m_Instance = this;

        s_InitialWindowData.width = width;
        s_InitialWindowData.height = height;
        s_InitialWindowData.title = title;
        s_InitialWindowData.resizable = windowResizable;
    }

    bool App::Init()
    {
        if (!Sonic::Window::init(s_InitialWindowData.width, s_InitialWindowData.height, 
                s_InitialWindowData.title, s_InitialWindowData.resizable))
            return false;

        if (glewInit() != GLEW_OK)
        {
            Window::destroy();
            return false;
        }

        Renderer2D::init();

        m_Scene = OnInit();
        m_Scene->AddListener(this, &App::OnWindowClosed);
        m_Scene->Init();
        
        return true;
    }

    void App::Exit()
    {
        OnExit();
    }

    void App::Run()
    {
        m_Running = true;

        const double secondsPerUpdate = 1.0 / 60.0;
        double deltaSeconds = 0.0;
        double dueUpdates = 0.0;

        double startTime = 0.0;
        double lastUpdate = 0.0;

#ifdef SONIC_DEBUG
        double fpsTimer = 0.0f;
        int frames = 0;
        int lastFpsLength = 0;
#endif

        while (m_Running)
        {
            dueUpdates += deltaSeconds / secondsPerUpdate;
            while (dueUpdates >= 1)
            {
                const double updateDeltaSeconds = startTime - lastUpdate;
                m_Scene->Update(static_cast<float>(updateDeltaSeconds));
                lastUpdate = startTime;
                dueUpdates--;
            }

            glClear(GL_COLOR_BUFFER_BIT);
            m_Scene->Render();
            Window::swapBuffers();
            frames++;

            const double endTime = Window::getTime();
            deltaSeconds = endTime - startTime;
            startTime = endTime;
            
#ifdef SONIC_DEBUG
            fpsTimer += deltaSeconds;
            if (fpsTimer > 1.0f)
            {
                fpsTimer -= 1.0f;
                std::string oldTitle = std::string(Window::getTitle());
                std::string framesStr = std::to_string(frames);
                std::string sub = oldTitle.substr(0, oldTitle.length() - lastFpsLength);
                Window::setTitle(oldTitle.substr(0, oldTitle.length() - lastFpsLength) + " (" + framesStr + ")");
                lastFpsLength = static_cast<int>(framesStr.length()) + 3;
                frames = 0;
            }
#endif

            Window::pollEvents();
        }
    }

    void App::OnWindowClosed(const WindowClosedEvent& event)
    {
        Stop();
    }

    void App::Stop()
    {
        m_Running = false;
    }

    App::~App()
    { 
        Window::destroy();

        if (m_Scene) 
            delete m_Scene;
    }
}