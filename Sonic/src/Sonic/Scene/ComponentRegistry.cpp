#include "ComponentRegistry.h"

using namespace Sonic;


void ComponentRegistry::DeactivateEntity(Entity entity)
{
	auto it = m_ComponentMap.find(entity);

	for (ComponentType type : it->second)
	{
		AnonymousComponentPool* pool = m_ComponentPools.at(type);
		pool->DeactivateEntity(entity, getComponentSize(type));
	}
}

void ComponentRegistry::ReactivateEntity(Entity entity)
{
	auto it = m_ComponentMap.find(entity);

	for (ComponentType type : it->second)
	{
		AnonymousComponentPool* pool = m_ComponentPools.at(type);
		pool->ReactivateEntity(entity, getComponentSize(type));
	}
}

void ComponentRegistry::RemoveEntity(Entity entity)
{
	auto it = m_ComponentMap.find(entity);

	if (it == m_ComponentMap.end())
		return;

	for (ComponentType type : it->second)
	{
		AnonymousComponentPool* pool = m_ComponentPools.at(type);
		pool->RemoveEntity(entity);
	}

	m_ComponentMap.erase(it);
}
