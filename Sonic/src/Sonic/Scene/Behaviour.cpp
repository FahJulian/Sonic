#pragma once
#include "Entity.h"
#include "Behaviour.h"

using namespace Sonic;


Entity Behaviour::GetEntity() const
{
	return m_Scene->ToEntity(m_EntityID);
}
