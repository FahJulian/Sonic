#pragma once
#include "Sonic/Log/Log.h"
#include "ComponentPool.h"

using namespace Sonic;


void AnonymousComponentPool::DeactivateEntity(Entity entity, size_t componentSize)
{
	size_t index = IndexOf(entity);

	if (m_InactiveSize == m_InactiveCapacity)
		IncreaseInactiveSize(componentSize);

	std::copy(m_Data + index * componentSize, m_Data + (index + 1) * componentSize, m_InactiveData + m_InactiveSize * componentSize);
	std::copy(m_Entities + index, m_Entities + index + 1, m_InactiveEntities + m_InactiveSize);

	std::copy(m_Data + (index + 1) * componentSize, m_Data + m_Size * componentSize, m_Data + index * componentSize);
	std::copy(m_Entities + index + 1, m_Entities + m_Size, m_Entities + index);

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

	m_Size--;
	m_InactiveSize++;
}

void AnonymousComponentPool::ReactivateEntity(Entity entity, size_t componentSize)
{
	size_t index = IndexOfInactive(entity);

	if (m_Size == m_Capacity)
		IncreaseSize(componentSize);

	std::copy(m_InactiveData + index * componentSize, m_InactiveData + (index + 1) * componentSize, m_Data + m_Size * componentSize);
	std::copy(m_InactiveEntities + index, m_InactiveEntities + index + 1, m_Entities + m_Size);

	std::copy(m_InactiveData + (index + 1) * componentSize, m_InactiveData + m_InactiveSize * componentSize, m_InactiveData + index * componentSize);
	std::copy(m_InactiveEntities + index + 1, m_InactiveEntities + m_InactiveSize, m_InactiveEntities + index);

	for (GroupViewInfo groupView : m_GroupViews)
		if (groupView.otherPool->HasEntity(entity))
			groupView.entities->push_back(entity);

	m_Size++;
	m_InactiveSize--;
}

bool AnonymousComponentPool::HasEntity(Entity entity)
{
	return IndexOf(entity) != NOT_FOUND;
}

size_t AnonymousComponentPool::IndexOf(Entity entity)
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

size_t AnonymousComponentPool::IndexOfInactive(Entity entity)
{
	size_t iteration = 0;
	while (iteration++ < m_InactiveSize)
	{
		if (m_InactiveCursor >= m_InactiveSize)
			m_InactiveCursor = 0;

		if (m_InactiveEntities[m_InactiveCursor] == entity)
			return m_InactiveCursor;

		m_InactiveCursor++;
	}

	return NOT_FOUND;
}

void AnonymousComponentPool::IncreaseSize(size_t componentSize)
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

void AnonymousComponentPool::IncreaseInactiveSize(size_t componentSize)
{
	m_InactiveCapacity = (size_t)((double)(m_InactiveCapacity + 1) * 1.2);

	uint8_t* newData = new uint8_t[m_InactiveCapacity * componentSize];
	std::copy(m_InactiveData, m_InactiveData + m_InactiveSize * componentSize, newData);
	delete[] m_InactiveData;
	m_InactiveData = newData;

	Entity* newEntities = new Entity[m_InactiveCapacity];
	std::copy(m_InactiveEntities, m_InactiveEntities + m_InactiveSize, newEntities);
	delete[] m_InactiveEntities;
	m_InactiveEntities = newEntities;
}
