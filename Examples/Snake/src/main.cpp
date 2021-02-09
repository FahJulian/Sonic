#pragma once
#include "Sonic/main.h"
#include "StartScene.h"
#include "GameScene.h"
#include "Settings.h"

Sonic::AppData Sonic::init()
{
	GameScene* gameScene = new GameScene();
	StartScene* startScene = new StartScene(gameScene);

	return {
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		WINDOW_TITLE,
		true,
		startScene
	};
}

const std::string& Sonic::resourceDir()
{
	static const std::string RESOURCE_DIR = "C:\\dev\\Sonic\\Examples\\Snake\\res\\";
	return RESOURCE_DIR;
}



/*

#pragma once
#include <stdint.h>
#include <unordered_map>
#include <limits>
#include <vector>
#include <functional>
#include <utility>

#include "Sonic/Log/Log.h"

//#include "Sonic/main.h"
//#include "StartScene.h"
//#include "GameScene.h"
//#include "Settings.h"
//
//Sonic::AppData Sonic::init()
//{
//	GameScene* gameScene = new GameScene();
//	StartScene* startScene = new StartScene(gameScene);
//
//	return {
//		WINDOW_WIDTH,
//		WINDOW_HEIGHT,
//		WINDOW_TITLE,
//		true,
//		startScene
//	};
//}
//
//const std::string& Sonic::resourceDir()
//{
//	static const std::string RESOURCE_DIR = "C:\\dev\\Sonic\\Examples\\Snake\\res\\";
//	return RESOURCE_DIR;
//}

#include <iostream>
#include <unordered_map>
#include <vector>

using Entity = uint32_t;
using ComponentType = uint16_t;

static ComponentType s_NextType = 0;

static size_t s_SizesSize;
static size_t* s_Sizes;

template<typename Component>
ComponentType RegisterComponent()
{
	ComponentType type = s_NextType++;
	size_t* newSizes = new size_t[s_SizesSize + 1];
	memcpy(newSizes, s_Sizes, s_SizesSize * sizeof(size_t));
	newSizes[type] = sizeof(Component);
	delete[] s_Sizes;
	s_Sizes = newSizes;
	s_SizesSize++;
	return type;
}

template<typename Component>
ComponentType getType()
{
	static ComponentType type = RegisterComponent<Component>();
	return type;
}

size_t getSize(ComponentType type)
{
	return s_Sizes[type];
}

class ComponentPool
{
	ComponentPool()
		: m_Size(0), m_Capacity(0), m_Cursor(0), m_Data(nullptr), m_Entities(nullptr)
	{
		SONIC_LOG_DEBUG("Constructing component pool");
	}

	template<typename Component>
	Component* GetComponent(Entity e)
	{
		size_t index = IndexOf(e);

		SONIC_LOG_DEBUG_ASSERT(index != NOT_FOUND, "Cant get Component from ComponentPool: Pool does not contain Entity");

		return reinterpret_cast<Component*>(m_Data) + index;
	}

	uint8_t* GetComponent(Entity entity, size_t componentSize)
	{
		size_t index = IndexOf(entity);

		SONIC_LOG_DEBUG_ASSERT(index != NOT_FOUND, "Cant remove Entity from ComponentPool: Pool does not contain Entity");

		return m_Data + index * componentSize;
	}

	template<typename Component, typename... Args>
	void AddComponent(Entity entity, Args&&... args)
	{
		if (m_Size == m_Capacity)
			IncreaseSize(sizeof(Component));

		new(reinterpret_cast<Component*>(m_Data + m_Size * sizeof(Component))) Component(std::forward<Args>(args)...);
		m_Entities[m_Size] = entity;

		m_Size++;
	}

	void AddComponent(Entity entity, uint8_t* data, size_t componentSize)
	{
		if (m_Size == m_Capacity)
			IncreaseSize(componentSize);

		for (size_t i = 0; i < componentSize; i++)
			m_Data[m_Size * componentSize + i] = data[i];

		m_Entities[m_Size] = entity;

		m_Size++;
	}

	void TransferEntity(Entity entity, ComponentPool* other, size_t componentSize)
	{
		uint8_t* data = GetComponent(entity, componentSize);
		other->AddComponent(entity, data, componentSize);
		RemoveEntity(entity, componentSize);
	}

	bool HasEntity(Entity entity)
	{
		return IndexOf(entity) != NOT_FOUND;
	}

	void RemoveEntity(Entity entity, size_t componentSize)
	{
		size_t index = IndexOf(entity);

		SONIC_LOG_DEBUG_ASSERT(index != NOT_FOUND, "Cant remove Entity from ComponentPool: Pool does not contain Entity");

		memcpy(m_Data + index * componentSize, m_Data + (index + 1) * componentSize, (m_Size - index - 1) * componentSize);
		memcpy(m_Entities + index, m_Entities + index + 1, (m_Size - index - 1) * sizeof(Entity));

		m_Size--;

		for (size_t* iteratorIndex : m_ActiveIteratorIndices)
		{
			if (*iteratorIndex >= index)
				(*iteratorIndex)--;
		}
	}

	size_t IndexOf(Entity entity)
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

	void IncreaseSize(size_t componentSize)
	{
		SONIC_LOG_DEBUG("Increasing component pool size");
		m_Capacity = (size_t)((double)(m_Capacity + 1) * 1.2);

		uint8_t* newData = new uint8_t[m_Capacity * componentSize];
		memcpy(newData, m_Data, m_Size * componentSize);
		delete[] m_Data;
		m_Data = newData;

		Entity* newEntities = new Entity[m_Capacity];
		memcpy(newEntities, m_Entities, m_Size * sizeof(Entity));
		delete[] m_Entities;
		m_Entities = newEntities;
	}

	void Destroy()
	{
		SONIC_LOG_DEBUG("Destroying component pool");
		delete[] m_Data;
		delete[] m_Entities;
	}

	size_t m_Size;
	size_t m_Capacity;
	size_t m_Cursor;
	uint8_t* m_Data;
	Entity* m_Entities;
	std::vector<size_t*> m_ActiveIteratorIndices;

	static const size_t NOT_FOUND = std::numeric_limits<size_t>::max();

	friend class Registry;
	friend class EntityView;
	template<typename> friend class ComponentView;
	template<typename> friend class PairView;
};

class Registry
{
public:
	Registry()
		: m_ComponentPoolsSize(0), m_ComponentPools(nullptr)
	{
	}

	ComponentPool* GetComponentPool(ComponentType type)
	{
		if (type >= m_ComponentPoolsSize)
		{
			ComponentPool* newComponentPools = new ComponentPool[(size_t)type + 1];
			memcpy(newComponentPools, m_ComponentPools, m_ComponentPoolsSize * sizeof(ComponentPool));
			delete[] m_ComponentPools;
			m_ComponentPools = newComponentPools;
			m_ComponentPoolsSize = (size_t)type + 1;
		}

		return m_ComponentPools + type;
	}

	template<typename Component, typename... Args>
	void AddComponent(Entity entity, Args&&... args)
	{
		ComponentType type = getType<Component>();
		ComponentPool* pool = GetComponentPool(type);
		pool->AddComponent<Component>(entity, std::forward<Args>(args)...);
		m_ComponentMap[entity].push_back(type);
	}

	template<typename Component>
	Component* GetComponent(Entity e)
	{
		ComponentPool* pool = GetComponentPool(getType<Component>());
		return pool->GetComponent<Component>(e);
	}

	template<typename Component>
	void RemoveComponent(Entity entity)
	{
		ComponentType type = getType<Component>();
		ComponentPool* pool = GetComponentPool(type);
		pool->RemoveEntity(entity, sizeof(Component));
		m_ComponentMap[entity].erase(std::remove(m_ComponentMap[entity].begin(), m_ComponentMap[entity].end(), type));
	}

	void TransferEntity(Entity entity, Registry* other)
	{
		auto it = m_ComponentMap.find(entity);

		if (it == m_ComponentMap.end())
			return;

		for (ComponentType type : it->second)
		{
			ComponentPool* pool = GetComponentPool(type);
			pool->TransferEntity(entity, other->GetComponentPool(type), getSize(type));
		}

		other->m_ComponentMap.insert(*it);
		m_ComponentMap.erase(it);
	}

	void RemoveEntity(Entity entity)
	{
		auto it = m_ComponentMap.find(entity);

		if (it == m_ComponentMap.end())
			return;

		for (ComponentType type : it->second)
		{
			ComponentPool* pool = GetComponentPool(type);
			pool->RemoveEntity(entity, getSize(type));
		}

		m_ComponentMap.erase(it);
	}

	~Registry()
	{
		for (int i = 0; i < m_ComponentPoolsSize; i++)
			m_ComponentPools[i].Destroy();
	}

	size_t m_ComponentPoolsSize;
	ComponentPool* m_ComponentPools;
	std::unordered_map<Entity, std::vector<ComponentType>> m_ComponentMap;
};

template<typename Component>
class PairView
{
	struct Pair
	{
		Entity entity;
		Component* component;
	};

	struct Iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using value_type = Pair;
		using reference = Pair&;
		using pointer = Pair*;

		size_t index;
		ComponentPool * pool;

		Iterator(ComponentPool* pool, size_t index)
			: pool(pool), index(index)
		{
			pool->m_ActiveIteratorIndices.push_back(&this->index);
			SONIC_LOG_DEBUG("Constructing iterator");
		}

		value_type operator*() { return { pool->m_Entities[index], reinterpret_cast<Component*>(pool->m_Data) + index }; }

		Iterator& operator++() { index++; return *this; }
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

		friend bool operator==(const Iterator& a, const Iterator& b) { return a.index == b.index; }
		friend bool operator!=(const Iterator& a, const Iterator& b)
		{
			return a.index != b.index;
		}

		~Iterator()
		{
			pool->m_ActiveIteratorIndices.erase(std::remove(pool->m_ActiveIteratorIndices.begin(), pool->m_ActiveIteratorIndices.end(), &index));
		}
	};

public:
	PairView(Registry* registry)
		: m_Pool(registry->GetComponentPool(getType<Component>()))
	{
	}

	Iterator begin() { return Iterator(m_Pool, 0); }
	Iterator end() { return Iterator(m_Pool, m_Pool->m_Size); }

	void ForEach(std::function<void(Entity entity, Component* component)> function)
	{
		for (auto [entity, component] : *this)
			function(entity, component);
	}

	size_t Size()
	{
		return m_Pool->m_Size;
	}

	ComponentPool* m_Pool;
};

template<typename Component>
class ComponentView
{
	struct Iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using value_type = Component;
		using reference = Component&;
		using pointer = Component*;

		size_t index;
		ComponentPool* pool;

		Iterator(ComponentPool* pool, size_t index)
			: pool(pool), index(index)
		{
			SONIC_LOG_DEBUG("Constructing iterator");
			pool->m_ActiveIteratorIndices.push_back(&this->index);
		}

		pointer operator*() { return reinterpret_cast<Component*>(pool->m_Data) + index; }

		Iterator& operator++() { index++; return *this; }
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

		friend bool operator==(const Iterator& a, const Iterator& b) { return a.index == b.index; }
		friend bool operator!=(const Iterator& a, const Iterator& b)
		{
			return a.index != b.index;
		}

		~Iterator()
		{
			pool->m_ActiveIteratorIndices.erase(std::remove(pool->m_ActiveIteratorIndices.begin(), pool->m_ActiveIteratorIndices.end(), &index));
		}
	};

public:
	ComponentView(Registry* registry)
		: m_Pool(registry->GetComponentPool(getType<Component>()))
	{
	}

	Iterator begin() { return Iterator(m_Pool, 0); }
	Iterator end() { return Iterator(m_Pool, m_Pool->m_Size); }

	void ForEach(std::function<void(Component* component)> function)
	{
		for (auto component : *this)
			function(component);
	}

	size_t Size()
	{
		return m_Pool->m_Size;
	}

	ComponentPool* m_Pool;
};

class EntityView
{
	struct Iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using value_type = Entity;
		using reference = Entity&;
		using pointer = Entity*;

		size_t index;
		ComponentPool* pool;

		Iterator(ComponentPool* pool, size_t index)
			: pool(pool), index(index)
		{
			SONIC_LOG_DEBUG("Constructing iterator");
			pool->m_ActiveIteratorIndices.push_back(&this->index);
		}

		value_type operator*() { return pool->m_Entities[index]; }

		Iterator& operator++() { index++; return *this; }
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

		friend bool operator==(const Iterator& a, const Iterator& b) { return a.index == b.index; }
		friend bool operator!=(const Iterator& a, const Iterator& b)
		{
			return a.index != b.index;
		}

		~Iterator()
		{
			pool->m_ActiveIteratorIndices.erase(std::remove(pool->m_ActiveIteratorIndices.begin(), pool->m_ActiveIteratorIndices.end(), &index));
		}
	};

public:
	EntityView(Registry* registry, ComponentType type)
		: m_Pool(registry->GetComponentPool(type))
	{
	}

	Iterator begin() { return Iterator(m_Pool, 0); }
	Iterator end() { return Iterator(m_Pool, m_Pool->m_Size); }

	void ForEach(std::function<void(Entity entity)> function)
	{
		for (auto entity : *this)
			function(entity);
	}

	size_t Size()
	{
		return m_Pool->m_Size;
	}

	ComponentPool* m_Pool;
};

class Scene
{
public:
	Entity AddEntity()
	{
		static Entity nextEntity = 1;
		return nextEntity++;
	}

	void DeactivateEntity(Entity entity)
	{
		m_MainRegistry.TransferEntity(entity, &m_InactiveRegistry);
		// Dispatch Event
	}

	void ActivateEntity(Entity entity)
	{
		m_InactiveRegistry.TransferEntity(entity, &m_MainRegistry);
		// Dispatch Event
	}

	void RemoveEntity(Entity entity)
	{
		m_MainRegistry.RemoveEntity(entity);
		m_InactiveRegistry.RemoveEntity(entity);
		// Dispatch Event
	}

	template<typename Component, typename... Args>
	void AddComponent(Entity entity, Args&&... args)
	{
		m_MainRegistry.AddComponent<Component>(entity, std::forward<Args>(args)...);
		// Dispatch Event
	}

	template<typename Component>
	Component* GetComponent(Entity entity)
	{
		return m_MainRegistry.GetComponent<Component>(entity);
	}

	template<typename Component>
	void RemoveComponent(Entity entity)
	{
		m_MainRegistry.RemoveComponent<Component>(entity);
		// Dispatch Event
	}

	template<typename Component>
	PairView<Component> View()
	{
		static std::unordered_map<Scene*, PairView<Component>> views;

		auto it = views.find(this);
		if (it == views.end())
		{
			views.emplace(this, PairView<Component>(&m_MainRegistry));
			it = --views.end();
		}

		return it->second;
	}

	template<typename Component>
	ComponentView<Component> ViewComponents()
	{
		static std::unordered_map<Scene*, ComponentView<Component>> views;

		auto it = views.find(this);
		if (it == views.end())
		{
			views.emplace(this, ComponentView<Component>(&m_MainRegistry));
			it = --views.end();
		}

		return it->second;
	}

	template<typename Component>
	EntityView ViewEntities()
	{
		static std::unordered_map<Scene*, EntityView> views;

		auto it = views.find(this);
		if (it == views.end())
		{
			views.emplace(this, EntityView(&m_MainRegistry, getType<Component>()));
			it = --views.end();
		}

		return it->second;
	}

	Registry m_MainRegistry;
	Registry m_InactiveRegistry;
};

struct C1
{
	float f;
	const char* t;

	C1(float f, const char* t)
		: f(f), t(t)
	{
	}

	C1(const C1& other)
		: f(other.f), t(other.t)
	{
		SONIC_LOG_DEBUG("Copying component");
	}
};

struct C2
{
	int i;

	C2(int i)
		: i(i)
	{
	}
};

int main()
{
	Scene scene;

	Entity e1 = scene.AddEntity();
	Entity e2 = scene.AddEntity();

	scene.AddComponent<C1>(e1, 1.0f, "Test");

	PairView<C1> view = scene.View<C1>();

	for (auto [e, c] : view)
	{
		SONIC_LOG_DEBUG(e, ", ", c->f);
		scene.AddComponent<C1>(scene.AddEntity(), 2.5f, "A");
	}

	scene.AddComponent<C1>(e2, 1.0f, "Test");

	for (auto c : scene.ViewComponents<C1>())
		SONIC_LOG_DEBUG(c->f);

	for (auto e : scene.ViewEntities<C1>())
		SONIC_LOG_DEBUG(e);

	scene.AddComponent<C2>(e1, 1);

	C1* c1 = scene.GetComponent<C1>(e1);
	C2* c2 = scene.GetComponent<C2>(e1);

	scene.RemoveComponent<C1>(e1);

	scene.DeactivateEntity(e1);
	scene.RemoveEntity(e1);
}

*/