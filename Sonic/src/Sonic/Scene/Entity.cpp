#pragma once
#include "Entity.h"

using namespace Sonic;


static EntityID nextID = 1;


Entity::Entity(Scene* scene)
	: m_Scene(scene), m_ID(nextID++)
{
}

Entity::Entity(Scene* scene, EntityID id)
	: m_Scene(scene), m_ID(id)
{
}
