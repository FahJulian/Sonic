#include "../App.h"
#include "Scene.h"
#include "Components.h"
#include "ComponentView.h"
#include "../Window/Window.h"
#include "../Renderer/Camera2D.h"
#include "../Renderer/Renderer2D.h"

namespace Sonic {

    Scene::Scene()
        : m_ComponentPools(nullptr), m_ComponentPoolsSize(0), m_Camera(Camera2D(0, 0, 0, 0))
    {
    }

    Entity Scene::AddEntity()
    {
        Entity e = nextEntity++;
        m_EntityComponentMap[e] = std::vector<ComponentType>();
        m_EntityComponentMap[e].reserve(ENTITY_COMPONENT_MAP_VECTOR_RESERVE_STEP);
        return e;
    }

    void Scene::RemoveEntity(Entity entity)
    {
        m_EntitiesToDelete.emplace_back(entity);
    }

    void Scene::Init()
    {   
        OnInit();
    }

    void Scene::Update(float deltaTime)
    {
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

        #ifdef SONIC_DEBUG
        bool canDeleteEntities = true;
        for (int i = 0; i < m_ComponentPoolsSize; i++)
        {
            if (m_ComponentPools[i].m_ActiveIterators != 0)
                canDeleteEntities = false;
        }

        if (!canDeleteEntities)
        {
            SONIC_LOG_DEBUG("Exiting App: There are iterators over ComponentViews active at the end of the Update stage.")
            App::get()->Stop();
        }
        else
        #endif

        for (Entity e : m_EntitiesToDelete)
        {
            std::vector<ComponentType> componentsOfEntity = m_EntityComponentMap[e];
            for (ComponentType type : componentsOfEntity)
                m_ComponentPools[type - 1].Remove(e);
            m_EntityComponentMap.erase(e);
        }
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

        OnRender();

        Renderer2D::endScene();
    }

}
