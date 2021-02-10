#pragma once
#include "Sonic/Log/Log.h"
#include "ComponentPool.h"

using namespace Sonic;


uint8_t* ComponentPool::GetComponent(Entity entity, size_t componentSize)
{
	size_t index = IndexOf(entity);

	SONIC_LOG_DEBUG_ASSERT(index != NOT_FOUND, "Cant remove Entity from ComponentPool: Pool does not contain Entity");

	return m_Data + index * componentSize;
}

void ComponentPool::AddComponent(Entity entity, uint8_t* data, size_t componentSize)
{
	if (m_Size == m_Capacity)
		IncreaseSize(componentSize);

	std::copy(data, data + componentSize, m_Data + m_Size * componentSize);

	m_Entities[m_Size] = entity;

	m_Size++;
}

void ComponentPool::TransferEntity(Entity entity, ComponentPool* other, size_t componentSize)
{
	uint8_t* data = GetComponent(entity, componentSize);
	other->AddComponent(entity, data, componentSize);
	RemoveEntity(entity, componentSize);
}

bool ComponentPool::HasEntity(Entity entity)
{
	return IndexOf(entity) != NOT_FOUND;
}

void ComponentPool::RemoveEntity(Entity entity, size_t componentSize)
{
	size_t index = IndexOf(entity);

	SONIC_LOG_DEBUG_ASSERT(index != NOT_FOUND, "Cant remove Entity from ComponentPool: Pool does not contain Entity");

	std::copy(m_Data + (index + 1) * componentSize, m_Data + m_Size * componentSize, m_Data + index * componentSize);
	std::copy(m_Entities + index + 1, m_Entities + m_Size, m_Entities + index);

	m_Size--;

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

size_t ComponentPool::IndexOf(Entity entity)
{
	size_t iteration = 0;
	while (iteration++ < m_Size)
	{
		if (m_Cursor >= m_Size)
			m_Cursor = 0;

		if (m_Entities[m_Cursor] == entity)
			return m_Cursor;

		m_Cursor++;
	}

	return NOT_FOUND;
}

void ComponentPool::IncreaseSize(size_t componentSize)
{
	m_Capacity = (size_t)((double)(m_Capacity + 1) * 1.2);

	uint8_t* newData = new uint8_t[m_Capacity * componentSize];
	std::copy(m_Data, m_Data + m_Size * componentSize, newData);
	delete[] m_Data;
	m_Data = newData;

	Entity* newEntities = new Entity[m_Capacity];
	std::copy(m_Entities, m_Entities + m_Size, newEntities);
	delete[] m_Entities;
	m_Entities = newEntities;
}

void ComponentPool::Destroy()
{
	delete[] m_Data;
	delete[] m_Entities;
}
