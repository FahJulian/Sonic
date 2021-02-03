#include "BehaviourPool.h"

using namespace Sonic;


bool BaseBehaviourPool::HasEntity(EntityID entity)
{
	return IndexOf(entity) != -1;
}

void BaseBehaviourPool::RemoveEntity(EntityID entity)
{
	m_ToRemove.insert(IndexOf(entity));
}

int BaseBehaviourPool::IndexOf(EntityID entity)
{
	static unsigned int index = 0;

	unsigned int iteration = 0;
	while (iteration < m_Entites.size())
	{
		if (index >= m_Entites.size())
			index = 0;

		if (m_Entites.at(index) == entity)
			return index;

		index++;
		iteration++;
	}

	return -1;
}

void BaseBehaviourPool::UpdateBehaviours(float deltaTime)
{
	for (Behaviour* behaviour : m_Behaviours)
		behaviour->OnUpdate(deltaTime);
}

void BaseBehaviourPool::UpdatePool()
{
	for (int index : m_ToRemove)
	{
		m_Entites.erase(m_Entites.begin() + index);
		m_Behaviours.at(index)->OnDestroy();
		m_Behaviours.erase(m_Behaviours.begin() + index);
	}

	for (auto& [entity, behaviour] : m_ToAdd)
	{
		m_Entites.push_back(entity);
		m_Behaviours.push_back(behaviour);
		behaviour->m_Scene = m_Scene;
		behaviour->m_EntityID = entity;
		behaviour->OnInit();
	}

	m_ToRemove.clear();
	m_ToAdd.clear();
}
