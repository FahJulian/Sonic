#pragma once
#include "Entity.h"

static Sonic::EntityID nextID = 0;

namespace Sonic {

	Entity::Entity(Scene* scene)
		: m_Scene(scene), m_ID(nextID++)
	{
	}

	Entity::Entity(Scene* scene, EntityID id)
		: m_Scene(scene), m_ID(id)
	{
	}

}