#include "App.h"
#include "Renderer/Renderer2D.h"

struct InitialWindowData
{
    int width;
    int height;
    const char* title;
};

static InitialWindowData s_InitialWindowData;

namespace Sonic {
    
    App* App::m_Instance;

    App::App(int width, int height, const char* title)
        : m_Scene(nullptr), m_Running(false)
    {
        if (m_Instance != nullptr)
            return;
       
        App::m_Instance = this;

        s_InitialWindowData.width = width;
        s_InitialWindowData.height = height;
        s_InitialWindowData.title = title;
    }

    bool App::Init()
    {
        if (!Sonic::Window::init(s_InitialWindowData.width, s_InitialWindowData.height, s_InitialWindowData.title))
            return false;

        if (glewInit() != GLEW_OK)
        {
            Window::destroy();
            return false;
        }

        Window::onWindowClosed->AddListener<App, &App::OnWindowClosed>(this);

        Renderer2D::init();

        m_Scene = OnInit();
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

            const double endTime = Window::getTime();
            deltaSeconds = endTime - startTime;
            startTime = endTime;
            
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