#include "ComponentRegistry.h"

using namespace Sonic;


void ComponentRegistry::DeactivateEntity(Entity entity)
{
	auto it = m_ComponentMap.find(entity);

	for (ComponentType type : it->second)
	{
		ComponentPool* pool = m_ComponentPools.at(type);
		pool->DeactivateEntity(entity);
	}
}

void ComponentRegistry::ReactivateEntity(Entity entity)
{
	auto it = m_ComponentMap.find(entity);

	for (ComponentType type : it->second)
	{
		ComponentPool* pool = m_ComponentPools.at(type);
		pool->ReactivateEntity(entity);
	}
}

void ComponentRegistry::RemoveEntity(Entity entity)
{
	auto it = m_ComponentMap.find(entity);

	if (it == m_ComponentMap.end())
		return;

	for (ComponentType type : it->second)
	{
		ComponentPool* pool = m_ComponentPools.at(type);
		pool->RemoveEntity(entity);
	}

	m_ComponentMap.erase(it);
}

void ComponentRegistry::Destroy()
{
	for (ComponentPool* pool : m_ComponentPools)
		delete pool;

	m_ComponentPools.clear();
	m_ComponentMap.clear();
}
