#pragma once
#include <set>
#include <vector> 
#include <unordered_map>
#include <unordered_set>
#include "Sonic/Log/Log.h"
#include "Sonic/Renderer/Camera2D.h"
#include "Sonic/Event/Event.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Entity.h"
#include "Component.h"
#include "ComponentPool.h"
#include "ComponentView.h"
//#include "Behaviour.h"

static const int COMPONENT_POOL_ARRAY_RESERVE_STEP = 10;
static const unsigned short ENTITY_COMPONENT_MAP_VECTOR_RESERVE_STEP = 6;
static const int EVENT_DISPATCHER_RESERVE_STEP = 10;

static Sonic::Entity nextEntity = 1;

namespace Sonic {

    class Behaviour;

    /**
    * The Scene is responsible of managin Entities and their Components by 
    * adding and removing them to ComponentPools. It also holds EventDispatchers
    * and adds EventListeners to those EventDispatchers. It should be subclassed
    * by the Scene of the crGame.
    */
    class Scene
    {
    public:
        virtual void CheckCollisions() {}

        /**
        * Constructs a new Scene
        */
        Scene();

    private:
        /**
        * Initializes the Scene.
        */
        void Init();

        /**
        * Updates the Scene. Runs system updating the componentsAt the end of the Update, all
        * components that were removed during the Update are removed at once.
        * 
        * @param deltaTime The time that has passed since the last update in seconds
        */
        void Update(float deltaTime);

        /**
        * Draws all scene objects to the screen. 
        */
        void Render();

    protected:
        /**
        * Virtual method that should be overridden by the Subclass to initialize all entities and
        * register event listeners
        */
        virtual void OnInit() = 0;

        /**
        * Virtual method that should be override by the Subclass to update all entities in the scene
        * that are not automatically updated by the Baseclass
        * 
        * @param deltaTime The time that has passed since the last update in seconds
        */
        virtual void OnUpdate(float deltaTime) = 0;

        /**
        * Virtual method that should be overriden by the Subclass to draw all entities to the screen
        * that are not automatically drawn by the Baseclass
        */
        virtual void OnRender() = 0;

    public:
        /**
        * Registers a new entity within this scene.
        * 
        * @return The ID of the new Entity
        */
        Entity AddEntity();

        /**
        * Removes an Entity with the given Entity ID and all its components and behaviours 
        * from the scene. The entities get removed at the end of the current update 
        * phase together to avoid memory leaks from iterators during the update phase.
        * 
        * @param entity The Entity ID of the Entity that should be removed
        */
        void RemoveEntity(Entity entity);

        /**
        * Adds the given component to the given entity
        *
        * @param entity The Entity ID of the Entity that the Component should be added to
        * @param component The component that should be added to the entity
        */
        template<typename Component>
        void AddComponent(Entity entity, const Component& component)
        {
            GetComponentPool<Component>()->Add(entity, component);
            m_EntityComponentMap[entity].insert(Component::getComponentType());
        }

        /**
        * Adds a component of the given type with the given constructor arguments to the 
        * given entity
        * 
        * @param entity The Entity ID of the Entity that the Component should be added to
        * @param args The arguments to pass to the Components constructor
        */
        template<typename Component, typename... Args>
        void AddComponent(Entity entity, Args&&... args)
        {
            ComponentPool* p = GetComponentPool<Component>();
            p->Add<Component>(entity, std::forward<Args>(args)...);
            m_EntityComponentMap[entity].insert(Component::getComponentType());
        }

        /**
        * Searches the scene for a component of the given type that is part of
        * the given entity. The returned pointer may only be valid during the
        * current update phase because the buffers holding the components may
        * move their data resulting in the pointer pointing to freed memory.
        * If this method is called right after Scene::HasComponent() with the 
        * same entity, the scene is not searched twice. Thus, calling 
        * HasComponent() and the GetComponent() is not slower that calling 
        * GetComponent() and cecking if the result is NULL.
        * 
        * @param entity The entity for whos component to search or NULL
        */
        template<typename Component>
        Component* GetComponent(Entity entity)
        {
            ComponentPool* p = GetComponentPool<Component>();
            return p->Get<Component>(entity);
        }

        /**
        * Checks if the given Entity has a component of the given type.
        * If this method is called right before Scene::GetComponent() with the 
        * same entity, the scene is not searched twice. Thus, calling 
        * HasComponent() and the GetComponent() is not slower that calling 
        * GetComponent() and cecking if the result is NULL.
        * 
        * @param entity The entity whos components to check
        */
        template<typename Component>
        bool HasComponent(Entity entity)
        {
            return GetComponentPool<Component>()->Has(entity);
        }

        /**
        * Removes the component that is part of the given entity. Components are
        * removed only at the end of the current update phase to avoid memory leaks
        *
        * @param entity The entity whos componed should be removed
        */
        template<typename Component>
        void RemoveComponent(Entity entity)
        {
            ComponentType type = Component::getComponentType();
            std::unordered_set<ComponentType>& components = m_EntityComponentMap[entity];
            components.erase(std::find(components.begin(), components.end(), type));
            m_ComponentsToDelete[type].emplace_back(entity);
        }

        /**
        * Returns a ComponentView of all the components of the given Component type.
        * It should mainly be used to be iterated over with either ComponentView::ForEach()
        * or with ComponentView::Iterator. It is important to use references to avoid copying
        */
        template<typename Component>
        ComponentView<Component> View()
        {
            ComponentPool* p = GetComponentPool<Component>();
            return p->ToComponentView<Component>();
        }

        /**
        * Creates a new instance of the given behaviour type and passes it to the method that
        * adds it to the scene
        * 
        * @param entity The entity the behaviour should be added to
        */
        template<typename DerivedBehaviour>
        void AddBehaviour(Entity entity)
        {
            DerivedBehaviour* behaviour = new DerivedBehaviour();
            AddBehaviour(entity, static_cast<Behaviour*>(behaviour));
        }

        /**
        * Creates a new instance of the given behaviour type and passes it to the method that
        * adds it to the scene
        * 
        * @param entity The entity the behaviour should be added to
        * @param args The arguments to pass to the behaviours constructor
        */
        template<typename DerivedBehaviour, typename... Args>
        void AddBehaviour(Entity entity, Args&&... args)
        {
            DerivedBehaviour* behaviour = new DerivedBehaviour(std::forward<Args>(args)...);
            AddBehaviour(entity, static_cast<Behaviour*>(behaviour));
        }

        /**
        * Adds an event listener of the given type to the scene. If gets called every time an
        * event of the given type is dispatched. This method should be used to add 
        * functions, not methods.
        * 
        * @param listener The listener to add
        */
        template<typename Event>
        void AddEventListener(std::function<void(const Event&)> listener)
        {
            GetEventDispatcher<Event>()->AddListener(listener);
        }

        /**
        * Adds an event listener of the given type to the scene. If gets called every time an
        * event of the given type is dispatched. This method should be used to add
        * methods, not functions.
        *
        * @param obj Pointer to the object of the method
        * @param method Pointer to the method
        */
        template<typename F, typename Event>
        void AddEventListener(F* const obj, void(F::* method)(const Event&))
        {
            GetEventDispatcher<Event>()->AddListener(obj, method);
        }

        /**
        * Adds a listener to the EventDispatcher. This method should be used to add
        * functions, not methods. This adds a removable listener, meaning that the
        * returned int ptr can later be used to remove this listener.
        *
        * @param listener Pointer to the function
        *
        * @return pointer to the index of the function within this EventDispatcher. Can be used
        *         to remove the function later.
        */
        template<typename Event>
        std::shared_ptr<int> AddRemovableListener(std::function<void(const Event&)> listener)
        {
            return GetEventDispatcher<Event>()->AddRemovableListener(listener);
        }

        /**
        * Adds a listener to the EventDispatcher. This method should be used to add
        * methods, not functions. This adds a removable listener, meaning that the
        * returned int ptr can later be used to remove this listener.
        *
        * @param obj Pointer to the object of the method
        * @param method Pointer to the method
        *
        * @return pointer to the index of the method within this EventDispatcher. Can be used
        *         to remove the method later.
        */
        template<typename F, typename Event>
        std::shared_ptr<int> AddRemovableListener(F* const obj, void(F::* method)(const Event&))
        {
            return GetEventDispatcher<Event>()->AddRemovableListener(obj, method);
        }
        
        /**
        * Dispatches the given Event to all event listeners that are registert for
        * that event type.
        */
        template<typename Event>
        void DispatchEvent(const Event& e)
        {
            GetEventDispatcher<Event>()->Dispatch(e);
        }

        /**
        * Removes the listener at the specified index
        *
        * @param index The index of the listener to remove
        */
        template<typename Event>
        void RemoveEventListener(const std::shared_ptr<int>& index)
        {
            GetEventDispatcher<Event>()->Remove(index);
        }

    private:
        /**
        * Assigns the entity and this scene to the given behaviour and adds the behaviour to the scene
        */
        void AddBehaviour(Entity entity, Behaviour* behaviour);

        /**
        * Searches all EventDispatchers of one of the given Event type. If none is found,
        * a new one gets added.
        * 
        * @return The found or added EventDispatcher
        */
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

        /**
        * Searches all ComponentPools of one of the given Component type. If none is found,
        * a new one gets added.
        * 
        * @return Pointer to the found or added ComponentPool
        */
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
                SONIC_LOG_DEBUG("Scene: Reallocating space for ComponentPools")

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
        std::unordered_map<ComponentType, std::set<Entity>> m_ComponentsToDelete;

        std::unordered_map<EventType, intptr_t> m_EventDispatchers;

        std::vector<Behaviour*> m_Behaviours;
        std::set<int> m_BehavioursToDelete;

        Camera2D m_Camera;

        bool m_Initialized;

        friend class App;
    };

}
