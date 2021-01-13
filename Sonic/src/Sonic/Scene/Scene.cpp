#include <typeinfo>
#include "Sonic/Renderer/Renderer2D.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Renderer/Camera2D.h"
#include "Sonic/App.h"
#include "Scene.h"
#include "Components.h"
#include "ComponentView.h"
#include "Behaviour.h"


namespace Sonic {

    Scene::Scene()
        : m_ComponentPools(nullptr), m_ComponentPoolsSize(0), 
        m_Camera(Camera2D(0, Window::getWidth(), 0, Window::getHeight())), m_Initialized(false)
    {
        m_EventDispatchers.reserve(EVENT_DISPATCHER_RESERVE_STEP);
    }

    Entity Scene::AddEntity()
    {
        Entity e = nextEntity++;
        m_EntityComponentMap[e].reserve(ENTITY_COMPONENT_MAP_VECTOR_RESERVE_STEP);
        return e;
    }

    void Scene::RemoveEntity(Entity entity)
    {
        for (ComponentType t : m_EntityComponentMap[entity])
            m_ComponentsToDelete[t].insert(entity);
        m_EntityComponentMap.erase(entity);

        for (int i = 0; i < m_Behaviours.size(); i++)
            if (m_Behaviours.at(i)->entity == entity)
                m_BehavioursToDelete.insert(i);
    }

    void Scene::AddBehaviour(Entity entity, Behaviour* behaviour)
    {
        std::cout << "Adding Behaviour : " << typeid(*behaviour).name() << std::endl;
        behaviour->scene = this;
        behaviour->entity = entity;
        m_Behaviours.push_back(static_cast<Behaviour*>(behaviour));

        if (m_Initialized)
            behaviour->OnInit();
    }

    void Scene::Init()
    {   
        OnInit();

        for (Behaviour* b : m_Behaviours)
            b->OnInit();
        
        m_Initialized = true;
    }

    void Scene::Update(float deltaTime)
    {
        SONIC_LOG_DEBUG("Updating")
        for (auto& [entity, component] : View<Camera2DComponent>())
        {
            if (HasComponent<Transform2DComponent>(entity)) 
            {
                auto* t = GetComponent<Transform2DComponent>(entity);
                component.camera.SetPosition(t->position);
                component.camera.SetRotation(t->rotation);
                m_Camera = component.camera;
            }
        }

        OnUpdate(deltaTime);

        for (Behaviour* behaviour : m_Behaviours)
            behaviour->OnUpdate(deltaTime);

        CheckCollisions();

        // Erase all Components that were deleted during the update phase
        // Needs to be done at the end of the update phase because there
        // may be iterators that will cause Segfaults during the update phase
        for (auto& [type, entities] : m_ComponentsToDelete)
            m_ComponentPools[type - 1].Remove(entities);
        m_ComponentsToDelete.clear();

        for (std::set<int>::reverse_iterator iter = m_BehavioursToDelete.rbegin(); 
            iter != m_BehavioursToDelete.rend(); iter++)
        {
            m_Behaviours.at(*iter)->OnDestroy();
            m_Behaviours.erase(m_Behaviours.begin() + *iter);
        }
        m_BehavioursToDelete.clear();
    }

    void Scene::Render()
    {
        Renderer2D::startScene(&m_Camera);

        for (auto& [entity, component] : View<SpriteComponent>())
        {
            auto* t = GetComponent<Transform2DComponent>(entity);
            if (t->rotation != 0)
                Renderer2D::drawRotatedRect(t->position, t->scale, t->rotation, component.sprite);
            else
                Renderer2D::drawRect(t->position, t->scale, component.sprite);
        }

        for (auto& [entity, component] : View<ColorComponent>())
        {
            auto* t = GetComponent<Transform2DComponent>(entity);
            if (t->rotation != 0)
                Renderer2D::drawRotatedRect(t->position, t->scale, t->rotation, component.color);
            else
                Renderer2D::drawRect(t->position, t->scale, component.color);
        }

        for (auto& [entity, component] : View<ColoredSpriteComponent>())
        {
            auto* t = GetComponent<Transform2DComponent>(entity);
            if (t->rotation != 0)
                Renderer2D::drawRotatedRect(t->position, t->scale, t->rotation, component.sprite, component.color);
            else
                Renderer2D::drawRect(t->position, t->scale, component.sprite, component.color);
        }

        for (auto& behaviour : m_Behaviours)
            behaviour->OnRender();

        OnRender();

        Renderer2D::endScene();
    }

}
