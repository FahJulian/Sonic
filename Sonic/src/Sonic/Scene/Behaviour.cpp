#pragma once
#include "Entity.h"
#include "Behaviour.h"

namespace Sonic {

	Entity Behaviour::GetEntity() const
	{
		//static Entity entity = m_Scene->ToEntity(m_EntityID);
		//return entity;
		return m_Scene->ToEntity(m_EntityID);
	}

}
