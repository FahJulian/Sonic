#include "ComponentRegistry.h"

using namespace Sonic;


ComponentPool* ComponentRegistry::GetComponentPool(ComponentType type)
{
	while (type >= m_ComponentPools.size())
		m_ComponentPools.push_back(new ComponentPool());

	return m_ComponentPools.at(type);
}

bool ComponentRegistry::HasEntity(Entity entity)
{
	return m_ComponentMap.find(entity) != m_ComponentMap.end();
}

void ComponentRegistry::TransferEntity(Entity entity, ComponentRegistry* other)
{
	auto it = m_ComponentMap.find(entity);

#ifdef SONIC_DEBUG
	if (it == m_ComponentMap.end())
		return;
#endif

	for (ComponentType type : it->second)
	{
		ComponentPool* pool = GetComponentPool(type);
		pool->TransferEntity(entity, other->GetComponentPool(type), getComponentSize(type));
	}

	other->m_ComponentMap.insert(*it);
	m_ComponentMap.erase(it);
}

void ComponentRegistry::RemoveEntity(Entity entity)
{
	auto it = m_ComponentMap.find(entity);

	if (it == m_ComponentMap.end())
		return;

	for (ComponentType type : it->second)
	{
		ComponentPool* pool = GetComponentPool(type);
		pool->RemoveEntity(entity, getComponentSize(type));
	}

	m_ComponentMap.erase(it);
}

ComponentRegistry::~ComponentRegistry()
{
	for (ComponentPool* pool : m_ComponentPools)
		pool->Destroy();
}
