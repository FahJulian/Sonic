#pragma once
#include <set>
#include <vector> 
#include <unordered_map>
#include <unordered_set>
#include "Entity.h"
#include "Component.h"
#include "ComponentPool.h"
#include "ComponentView.h"
#include "Sonic/Log/Log.h"
#include "Sonic/Renderer/Camera2D.h"
#include "Sonic/Event/Event.h"
#include "Sonic/Event/EventDispatcher.h"

static const int COMPONENT_POOL_ARRAY_RESERVE_STEP = 10;
static const unsigned short ENTITY_COMPONENT_MAP_VECTOR_RESERVE_STEP = 6;
static const int EVENT_DISPATCHER_RESERVE_STEP = 10;

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
            m_EntityComponentMap[entity].insert(Component::getComponentType());
        }

        template<typename Component, typename... Args>
        void AddComponent(Entity entity, Args&&... args)
        {
            ComponentPool* p = GetComponentPool<Component>();
            p->Add<Component>(entity, std::forward<Args>(args)...);
            m_EntityComponentMap[entity].insert(Component::getComponentType());
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
            ComponentType type = Component::getComponentType();
            std::unordered_set<ComponentType>& components = m_EntityComponentMap[entity];
            components.erase(std::find(components.begin(), components.end(), type));
            m_ToDelete[type].emplace_back(entity);
        }

        template<typename Component>
        ComponentView<Component> View()
        {
            ComponentPool* p = GetComponentPool<Component>();
            return p->ToComponentView<Component>();
        }

        template<typename Event>
        void AddEventListener(std::function<void(const Event&)> listener)
        {
            GetEventDispatcher<Event>()->AddListener(listener);
        }

        template<typename F, typename Event>
        void AddEventListener(F* const obj, void(F::* method)(const Event&))
        {
            GetEventDispatcher<Event>()->AddListener(obj, method);
        }
        
        template<typename Event>
        void DispatchEvent(const Event& e)
        {
            GetEventDispatcher<Event>()->Dispatch(e);
        }

    private:
        template<typename Event>
        EventDispatcher<Event>* GetEventDispatcher()
        {
            EventType type = Event::getEventType();
            intptr_t ptr = m_EventDispatchers[type];

            if (!ptr)
            {
                EventDispatcher<Event>* newDispatcher = new EventDispatcher<Event>();
                m_EventDispatchers[type] = reinterpret_cast<intptr_t>(newDispatcher);
                return newDispatcher;
            }
            else
            {
                return reinterpret_cast<EventDispatcher<Event>*>(ptr);
            }
        }

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
        std::unordered_map<Entity, std::unordered_set<ComponentType>> m_EntityComponentMap;
        std::unordered_map<ComponentType, std::set<Entity>> m_ToDelete;
        std::unordered_map<EventType, intptr_t> m_EventDispatchers;

        Camera2D m_Camera;

        friend class App;
    };

}
