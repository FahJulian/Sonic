#pragma once
#include "Entity.h"
#include "Behaviour.h"

namespace Sonic {

	Entity Behaviour::GetEntity()
	{
		return m_Scene->ToEntity(m_Entity);
	}

}
