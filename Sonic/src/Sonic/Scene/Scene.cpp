#pragma once
#include "Sonic/Renderer/Renderer2D.h"
#include "Sonic/UI/Components.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"

namespace Sonic {

	Scene::Scene()
		: m_Camera(Camera2D(0, Window::getWidth(), 0, Window::getHeight())) 
	{
	}

	Entity Scene::AddEntity()
	{
		return Entity(this);
	}

	void Scene::RemoveEntity(EntityID entity)
	{
		for (BaseBehaviourPool* pool : m_BehaviourPools)
			if (pool->HasEntity(entity))
				pool->RemoveEntity(entity);

		for (BaseComponentPool* pool : m_ComponentPools)
			if (pool->HasEntity(entity))
				pool->RemoveEntity(entity);
	}

	Entity Scene::ToEntity(EntityID entity)
	{
		return { this, entity };
	}

	void Scene::Init()
	{
		Load();
		UpdatePools();
		OnInit();
	}

	void Scene::Update(float deltaTime)
	{
		OnUpdate(deltaTime);
		UpdateComponents(deltaTime);
		UpdateBehaviours(deltaTime);
		UpdatePools();
		PollCollisionEvents();
	}

	void Scene::UpdatePools()
	{
		for (BaseBehaviourPool* pool : m_BehaviourPools)
			pool->UpdatePool();

		for (BaseComponentPool* pool : m_ComponentPools)
			pool->OnUpdate();
	}

	void Scene::UpdateBehaviours(float deltaTime)
	{
		for (BaseBehaviourPool* pool : m_BehaviourPools)
			pool->UpdateBehaviours(deltaTime);
	}

	void Scene::UpdateComponents(float deltaTime)
	{
		for (auto [entity, component] : View<Camera2DComponent>())
		{
			if (!HasComponent<Transform2DComponent>(entity))
				return;

			auto* t = GetComponent<Transform2DComponent>(entity);
			component->camera.SetPosition(t->position);
			component->camera.SetRotation(t->rotation);
			m_Camera = component->camera;
		}
	}

	void Scene::Render()
	{
		Renderer2D::startScene(&m_Camera);

		for (auto [entity, component] : View<SpriteComponent>())
		{
			auto* t = GetComponent<Transform2DComponent>(entity);
			if (t->rotation != 0)
				Renderer2D::drawRotatedRect(t->position, t->scale, t->rotation, component->sprite);
			else
				Renderer2D::drawRect(t->position, t->scale, component->sprite);
		}

		for (auto [entity, component] : View<ColorComponent>())
		{
			auto* t = GetComponent<Transform2DComponent>(entity);
			if (t->rotation != 0)
				Renderer2D::drawRotatedRect(t->position, t->scale, t->rotation, component->color);
			else
				Renderer2D::drawRect(t->position, t->scale, component->color);
		}

		for (auto [entity, component] : View<ColoredSpriteComponent>())
		{
			auto* t = GetComponent<Transform2DComponent>(entity);
			if (t->rotation != 0)
				Renderer2D::drawRotatedRect(t->position, t->scale, t->rotation, component->sprite, component->color);
			else
				Renderer2D::drawRect(t->position, t->scale, component->sprite, component->color);
		}

		Renderer2D::endScene();
	}

}

