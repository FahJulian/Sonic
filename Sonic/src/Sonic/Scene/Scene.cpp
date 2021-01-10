#include "Sonic/Renderer/Renderer2D.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Renderer/Camera2D.h"
#include "Sonic/App.h"
#include "Scene.h"
#include "Components.h"
#include "ComponentView.h"

namespace Sonic {

    Scene::Scene()
        : m_ComponentPools(nullptr), m_ComponentPoolsSize(0), m_Camera(Camera2D(0, 0, 0, 0))
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
            m_ToDelete[t].insert(entity);
        m_EntityComponentMap.erase(entity);
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

        // Erase all Components that were deleted during the update phase
        // Needs to be done at the end of the update phase because there
        // may be iterators that will cause Segfaults during the update phase
        for (auto& [type, entities] : m_ToDelete)
            m_ComponentPools[type - 1].Remove(entities);
        m_ToDelete.clear();
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
