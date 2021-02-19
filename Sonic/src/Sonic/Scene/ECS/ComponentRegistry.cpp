#include "ComponentRegistry.h"

using namespace Sonic;


Entity ComponentRegistry::AddEntity()
{
	return m_NextEntity++;
}

Entity ComponentRegistry::AddEntity(EntityGroup group)
{
	Entity entity = m_NextEntity++;
	m_EntityGroups[group].push_back(entity);
	return entity;
}

EntityGroup ComponentRegistry::AddEntityGroup()
{
	return m_NextEntity++;
}

void ComponentRegistry::AddToGroup(EntityGroup group, Entity entity)
{
	m_EntityGroups[group].push_back(entity);
}

std::vector<Entity>* ComponentRegistry::GetGroup(EntityGroup group)
{
	return &m_EntityGroups[group];
}

void ComponentRegistry::DeactivateEntities(EntityGroup group)
{
	for (Entity entity : m_EntityGroups[group])
		DeactivateEntity(entity);
}

void ComponentRegistry::ReactivateEntities(EntityGroup group)
{
	for (Entity entity : m_EntityGroups[group])
		ReactivateEntity(entity);
}

ComponentPool* ComponentRegistry::GetComponentPool(ComponentType type)
{
	while (type >= m_ComponentPools.size())
		m_ComponentPools.push_back(new ComponentPool(static_cast<ComponentType>(m_ComponentPools.size())));

	return m_ComponentPools.at(type);
}

void ComponentRegistry::DeactivateEntity(Entity entity)
{
	EventDispatcher::dispatch(EntityDeactivatedEvent(entity));

	auto it = m_ComponentMap.find(entity);

	for (ComponentType type : it->second)
	{
		ComponentPool* pool = GetComponentPool(type);
		pool->DeactivateEntity(entity);
	}
}

void ComponentRegistry::ReactivateEntity(Entity entity)
{
	auto it = m_ComponentMap.find(entity);

	for (ComponentType type : it->second)
	{
		ComponentPool* pool = GetComponentPool(type);
		pool->ReactivateEntity(entity);
	}

	EventDispatcher::dispatch(EntityReactivatedEvent(entity));
}

void ComponentRegistry::RemoveEntity(Entity entity)
{
	EventDispatcher::dispatch(EntityRemovedEvent(entity));

	auto it = m_ComponentMap.find(entity);

	if (it == m_ComponentMap.end())
		return;

	for (ComponentType type : it->second)
	{
		ComponentPool* pool = GetComponentPool(type);
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

	m_NextEntity = 1;
	m_EntityGroups.clear();
}
