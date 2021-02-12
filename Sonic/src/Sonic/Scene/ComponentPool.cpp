#pragma once
#include "Sonic/Log/Log.h"
#include "ComponentPool.h"

using namespace Sonic;


std::vector<std::function<void(uint8_t*)>> ComponentPool::s_ComponentDestructors;
std::vector<size_t> ComponentPool::s_ComponentSizes;
ComponentType ComponentPool::s_NextComponentType = 0;


size_t ComponentPool::IndexOf(Entity entity, Data* data)
{
	size_t iteration = 0;
	while (iteration++ < data->size)
	{
		if (data->cursor >= data->size)
			data->cursor = 0;

		if (data->entities[data->cursor] == entity)
			return data->cursor;

		data->cursor++;
	}

	return NOT_FOUND;
}

void ComponentPool::IncreaseSize(Data* data)
{
	data->capacity = (size_t)((double)(data->capacity + 1) * 1.2);

	uint8_t* newData = new uint8_t[data->capacity * m_ComponentSize];
	std::copy(data->data, data->data + data->size * m_ComponentSize, newData);
	delete[] data->data;
	data->data = newData;

	Entity* newEntities = new Entity[data->capacity];
	std::copy(data->entities, data->entities + data->size, newEntities);
	delete[] data->entities;
	data->entities = newEntities;
}

void ComponentPool::MoveEntity(Data* src, Data* dst, size_t srcIndex)
{
	if (dst->size == dst->capacity)
		IncreaseSize(dst);

	std::copy(src->data + srcIndex * m_ComponentSize, src->data + (srcIndex + 1) * m_ComponentSize, dst->data + dst->size * m_ComponentSize);
	std::copy(src->entities + srcIndex, src->entities + srcIndex + 1, dst->entities + dst->size);

	std::copy(src->data + (srcIndex + 1) * m_ComponentSize, src->data + src->size * m_ComponentSize, src->data + srcIndex * m_ComponentSize);
	std::copy(src->entities + srcIndex + 1, src->entities + src->size, src->entities + srcIndex);
}

void ComponentPool::NotifyEntityRemoved(Entity entity, size_t index)
{
	for (size_t* iteratorIndex : m_ActiveIteratorIndices)
	{
		if (*iteratorIndex >= index)
			(*iteratorIndex)--;
	}

	for (GroupViewInfo groupView : m_GroupViews)
	{
		for (size_t i = 0, size = groupView.entities->size(); i < size; i++)
		{
			if (groupView.entities->at(i) == entity)
			{
				groupView.entities->erase(groupView.entities->begin() + i);
				for (size_t* iteratorIndex : *groupView.activeIteratorIndices)
					if (*iteratorIndex >= i)
						--(*iteratorIndex);
				return;
			}
		}
	}
}

void ComponentPool::NotifyEntityAdded(Entity entity)
{
	for (GroupViewInfo groupView : m_GroupViews)
		if (groupView.otherPool->HasEntity(entity))
			groupView.entities->push_back(entity);
}

void ComponentPool::DeactivateEntity(Entity entity)
{
	size_t index = IndexOf(entity, &m_Active);

	SONIC_LOG_DEBUG_ASSERT(index != NOT_FOUND, "Cant deactivate Component: No active Component of the given Entity exists.");

	MoveEntity(&m_Active, &m_Inactive, index);

	NotifyEntityRemoved(entity, index);

	m_Active.size--;
	m_Inactive.size++;
}

void ComponentPool::ReactivateEntity(Entity entity)
{
	size_t index = IndexOf(entity, &m_Inactive);

	SONIC_LOG_DEBUG_ASSERT(index != NOT_FOUND, "Cant reactivate Component: No inactive Component of the given Entity exists.");

	MoveEntity(&m_Inactive, &m_Active, index);

	NotifyEntityAdded(entity);

	m_Active.size++;
	m_Inactive.size--;
}

bool ComponentPool::HasEntity(Entity entity)
{
	return IndexOf(entity, &m_Active) != NOT_FOUND;
}

void ComponentPool::RemoveEntity(Entity entity)
{
	size_t index = IndexOf(entity, &m_Active);

	SONIC_LOG_DEBUG_ASSERT(index != NOT_FOUND, "Cant remove Entity from ComponentPool: Pool does not contain Entity");

	m_ComponentDestructor(m_Active.data + index * m_ComponentSize);

	std::copy(m_Active.data + (index + 1) * m_ComponentSize, m_Active.data + m_Active.size * m_ComponentSize, m_Active.data + index * m_ComponentSize);
	std::copy(m_Active.entities + index + 1, m_Active.entities + m_Active.size, m_Active.entities + index);

	NotifyEntityRemoved(entity, index);

	m_Active.size--;
}

ComponentPool::~ComponentPool()
{
	for (size_t i = 0; i < m_Active.size; i++)
		m_ComponentDestructor(m_Active.data + i * m_ComponentSize);

	for (size_t i = 0; i < m_Inactive.size; i++)
		m_ComponentDestructor(m_Inactive.data + i * m_ComponentSize);
}
