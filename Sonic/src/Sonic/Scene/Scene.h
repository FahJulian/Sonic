#pragma once
#include <vector> 
#include <unordered_map>
#include "../Log/Log.h"
#include "../Renderer/Camera2D.h"
#include "Entity.h"
#include "Component.h"
#include "ComponentPool.h"
#include "ComponentView.h"

static const int COMPONENT_POOL_ARRAY_RESERVE_STEP = 10;
static const int ENTITY_COMPONENT_MAP_VECTOR_RESERVE_STEP = 6;

static Sonic::Entity nextEntity = 1;

namespace Sonic {

    class Scene
    {
    public:
        Scene();

    private:
        void Init();
        void Update(float deltaTime);
        void Render();

    protected:
        virtual void OnInit() = 0;
        virtual void OnUpdate(float deltaTime) = 0;
        virtual void OnRender() = 0;

    public:
        Entity AddEntity();
        void RemoveEntity(Entity entity);

        template<typename Component>
        void AddComponent(Entity entity, const Component& component)
        {
            GetComponentPool<Component>()->Add(entity, component);
            m_EntityComponentMap[entity].emplace_back(Component::getComponentType());
        }

        template<typename Component, typename... Args>
        void AddComponent(Entity entity, Args&&... args)
        {
            ComponentPool* p = GetComponentPool<Component>();
            p->Add<Component>(entity, std::forward<Args>(args)...);
            m_EntityComponentMap[entity].emplace_back(Component::getComponentType());
        }

        template<typename Component>
        Component* GetComponent(Entity entity)
        {
            ComponentPool* p = GetComponentPool<Component>();
            return p->Get<Component>(entity);
        }

        template<typename Component>
        bool HasComponent(Entity entity)
        {
            return GetComponentPool<Component>()->Has(entity);
        }

        template<typename Component>
        void RemoveComponent(Entity entity)
        {
            GetComponentPool<Component>()->Remove(entity);
            std::vector<ComponentType> componentsOfEntity = m_EntityComponentMap[entity];
            componentsOfEntity.erase(std::find(componentsOfEntity.begin(), componentsOfEntity.end(), Component::getComponentType()));
        }

        template<typename Component>
        ComponentView<Component> View()
        {
            ComponentPool* p = GetComponentPool<Component>();
            return p->ToComponentView<Component>();
        }

    private:
        template<typename Component>
        ComponentPool* GetComponentPool()
        {
            ComponentType type = Component::getComponentType();

            if (type <= m_ComponentPoolsSize)
            {
                if (m_ComponentPools[type - 1].IsNull())
                    m_ComponentPools[type - 1] = ComponentPool::create<Component>();
                
                return &m_ComponentPools[type - 1];
            }
            else
            {
                SONIC_LOG_DEBUG("Scene: Reallocating space for CompoentPools")

                ComponentPool* newComponentPools = new ComponentPool[type + COMPONENT_POOL_ARRAY_RESERVE_STEP];
                std::copy(m_ComponentPools, m_ComponentPools + m_ComponentPoolsSize, newComponentPools);
                newComponentPools[type - 1] = ComponentPool::create<Component>();
                m_ComponentPoolsSize = type + COMPONENT_POOL_ARRAY_RESERVE_STEP;

                delete[] m_ComponentPools;
                m_ComponentPools = newComponentPools;

                return &m_ComponentPools[type - 1];
            }
        }

    private:
        ComponentPool* m_ComponentPools;
        ComponentType m_ComponentPoolsSize;
        std::unordered_map<Entity, std::vector<ComponentType>> m_EntityComponentMap;

        Camera2D m_Camera;

        std::vector<Entity> m_EntitiesToDelete;

        friend class App;
    };

}
