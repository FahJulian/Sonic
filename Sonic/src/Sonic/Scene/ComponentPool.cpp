#pragma once
#include "ComponentPool.h"

using namespace Sonic;


bool BaseComponentPool::HasEntity(EntityID entity)
{
	return IndexOf(entity) != -1;
}

void BaseComponentPool::RemoveEntity(EntityID entity)
{
	m_ToRemove.insert(IndexOf(entity));
}

int BaseComponentPool::IndexOf(EntityID entity)
{
	static unsigned int index = 0;

	unsigned int iteration = 0;
	while (iteration < m_Entities.size())
	{
		if (index >= m_Entities.size())
			index = 0;

		if (m_Entities.at(index) == entity)
			return index;

		index++;
		iteration++;
	}

	return -1;
}
