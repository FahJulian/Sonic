#pragma once
#include "Sonic/Renderer/Renderer2D.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"

namespace Sonic {

	Scene::Scene()
		: m_Camera(Camera2D(0, Window::getWidth(), 0, Window::getHeight())) 
	{
		AddListener(this, &Scene::OnMouseButtonReleased);
		AddListener(this, &Scene::OnMouseMoved);
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

		for (auto [entity, component] : View<Renderer2DComponent>())
		{
			auto* t = GetComponent<Transform2DComponent>(entity);
			
			Color* color = &component->color;
			Sprite* sprite = &component->sprite;

			if (HasComponent<HoverComponent>(entity))
			{
				auto* h = GetComponent<HoverComponent>(entity);
				if (h->hovered)
				{
					color = &h->color;
					sprite = &h->sprite;
				}
			}

			if (HasComponent<BorderComponent>(entity))
			{
				auto* b = GetComponent<BorderComponent>(entity);
				Renderer2D::drawRect(t->position, { t->scale.x - b->borderWeight, b->borderWeight }, t->rotation, b->color);
				Renderer2D::drawRect({ t->position.x + t->scale.x, t->position.y, t->position.z }, { -b->borderWeight, t->scale.y - b->borderWeight }, b->color);
				Renderer2D::drawRect({ t->position.x + t->scale.x, t->position.y + t->scale.y, t->position.z }, { -t->scale.x + b->borderWeight, -b->borderWeight }, t->rotation, b->color);
				Renderer2D::drawRect({ t->position.x, t->position.y + t->scale.y, t->position.z }, { b->borderWeight, -t->scale.y + b->borderWeight }, t->rotation, b->color);

				Renderer2D::drawRect({ t->position.x + b->borderWeight, t->position.y + b->borderWeight, t->position.z }, 
					{ t->scale.x - 2 * b->borderWeight, t->scale.y - 2 * b->borderWeight }, t->rotation, *sprite, *color);
			}
			else
			{
				Renderer2D::drawRect(t->position, t->scale, t->rotation, *sprite, *color);
			}
		}

		Renderer2D::endScene();
	}

	void Scene::OnMouseButtonReleased(const MouseButtonReleasedEvent& e)
	{
		for (auto [entity, component] : View<ClickListenerComponent>())
		{
			if (!HasComponent<Transform2DComponent>(entity))
				continue;

			auto* t = GetComponent<Transform2DComponent>(entity);
			if (e.x >= t->position.x && e.x < t->position.x + t->scale.x && e.y >= t->position.y && e.y < t->position.y + t->scale.y)
				component->listener(e);
		}
	}

	void Scene::OnMouseMoved(const MouseMovedEvent& e)
	{
		for (auto [entity, component] : View<HoverComponent>())
		{
			auto* t = GetComponent<Transform2DComponent>(entity);
			component->hovered = (e.x >= t->position.x && e.x < t->position.x + t->scale.x && 
								  e.y >= t->position.y && e.y < t->position.y + t->scale.y);
		}
	}

}
