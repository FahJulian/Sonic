#pragma once
#include "Sonic/Scene/ECS/Entity.h"

namespace Sonic {

    template<typename Component>
    struct ComponentAddedEvent
    {
        Entity entity;

        ComponentAddedEvent(Entity entity)
            : entity(entity)
        {
        }
    };

    template<typename Component>
    struct ComponentRemovedEvent
    {
        Entity entity;

        ComponentRemovedEvent(Entity entity)
            : entity(entity)
        {
        }
    };

    template<typename Component>
    struct ComponentDeactivatedEvent
    {
        Entity entity;

        ComponentDeactivatedEvent(Entity entity)
            : entity(entity)
        {
        }
    };

    template<typename Component>
    struct ComponentReactivatedEvent
    {
        Entity entity;

        ComponentReactivatedEvent(Entity entity)
            : entity(entity)
        {
        }
    };

    struct EntityDeactivatedEvent
    {
        Entity entity;

        EntityDeactivatedEvent(Entity entity)
            : entity(entity)
        {
        }
    };

    struct EntityReactivatedEvent
    {
        Entity entity;

        EntityReactivatedEvent(Entity entity)
            : entity(entity)
        {
        }
    };

    struct EntityRemovedEvent
    {
        Entity entity;

        EntityRemovedEvent(Entity entity)
            : entity(entity)
        {
        }
    };

}
