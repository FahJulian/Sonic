#include "ComponentRegistry.h"

using namespace Sonic;


ComponentPool* ComponentRegistry::GetComponentPool(ComponentType type)
{
	if (type >= m_ComponentPoolsSize)
	{
		ComponentPool* newComponentPools = new ComponentPool[(size_t)type + 1];
		std::copy(m_ComponentPools, m_ComponentPools + m_ComponentPoolsSize, newComponentPools);
		delete[] m_ComponentPools;
		m_ComponentPools = newComponentPools;
		m_ComponentPoolsSize = (size_t)type + 1;
	}

	return m_ComponentPools + type;
}

void ComponentRegistry::TransferEntity(Entity entity, ComponentRegistry* other)
{
	auto it = m_ComponentMap.find(entity);

	if (it == m_ComponentMap.end())
		return;

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
	for (int i = 0; i < m_ComponentPoolsSize; i++)
		m_ComponentPools[i].Destroy();
}
