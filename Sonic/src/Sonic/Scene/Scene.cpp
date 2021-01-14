#pragma once
#include "Sonic/Renderer/Renderer2D.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"

namespace Sonic {

	Scene::Scene()
		: m_Camera(Camera2D(0, Window::getWidth(), 0, Window::getHeight())) {}

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

		for (BaseBehaviourPool* pool : m_BehaviourPools)
			pool->UpdatePool();

		for (BaseComponentPool* pool : m_ComponentPools)
			pool->OnUpdate();

		OnInit();
	}

	void Scene::Update(float deltaTime)
	{
		for (auto entity : View<Camera2DComponent>())
		{
			auto* cameraComponent = View<Camera2DComponent>().GetComponent(entity);

			if (HasComponent<Transform2DComponent>(entity))
			{
				auto* t = GetComponent<Transform2DComponent>(entity);
				cameraComponent->camera.SetPosition(t->position);
				cameraComponent->camera.SetRotation(t->rotation);
				m_Camera = cameraComponent->camera;
			}
		}

		OnUpdate(deltaTime);

		for (BaseBehaviourPool* pool : m_BehaviourPools)
			pool->UpdateBehaviours(deltaTime);

		UpdatePools();

		CheckCollisions();

		UpdatePools();
	}

	void Scene::UpdatePools()
	{
		for (BaseBehaviourPool* pool : m_BehaviourPools)
			pool->UpdatePool();

		for (BaseComponentPool* pool : m_ComponentPools)
			pool->OnUpdate();
	}

	void Scene::Render()
	{
		Renderer2D::startScene(&m_Camera);

		for (auto entity : View<SpriteComponent>())
		{
			auto* spriteComponent = View<SpriteComponent>().GetComponent(entity);
			auto* t = GetComponent<Transform2DComponent>(entity);
			if (t->rotation != 0)
				Renderer2D::drawRotatedRect(t->position, t->scale, t->rotation, spriteComponent->sprite);
			else
				Renderer2D::drawRect(t->position, t->scale, spriteComponent->sprite);
		}

		for (auto& entity : View<ColorComponent>())
		{
			auto* colorComponent = View<ColorComponent>().GetComponent(entity);
			auto* t = GetComponent<Transform2DComponent>(entity);
			if (t->rotation != 0)
				Renderer2D::drawRotatedRect(t->position, t->scale, t->rotation, colorComponent->color);
			else
				Renderer2D::drawRect(t->position, t->scale, colorComponent->color);
		}

		for (auto& entity : View<ColoredSpriteComponent>())
		{
			auto* coloredSpriteComponent = View<ColoredSpriteComponent>().GetComponent(entity);
			auto* t = GetComponent<Transform2DComponent>(entity);
			if (t->rotation != 0)
				Renderer2D::drawRotatedRect(t->position, t->scale, t->rotation, coloredSpriteComponent->sprite, coloredSpriteComponent->color);
			else
				Renderer2D::drawRect(t->position, t->scale, coloredSpriteComponent->sprite, coloredSpriteComponent->color);
		}

		Renderer2D::endScene();
	}

}

