#pragma once
#include "Sonic/Renderer/Renderer2D.h"
#include "Sonic/Renderer/Font/FontRenderer.h"
#include "Sonic/Renderer/Font/Font.h"
#include "Sonic/UI/UIComponents.h"
#include "Sonic/UI/UIRenderer.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"

static Sonic::Font* font;
static Sonic::Font* font1;

namespace Sonic {

	Scene::Scene()
		: m_Camera(Camera2D(0, Window::getWidth(), 0, Window::getHeight())) 
	{
		AddListener(this, &Scene::OnMouseButtonReleased);
		AddListener(this, &Scene::OnMouseMoved);

		font = new Font("C:\\dev\\Sonic\\Sonic\\res\\fonts\\arial.ttf", 120);
		font1 = new Font("C:\\dev\\Sonic\\Sonic\\res\\fonts\\calibri.ttf", 40);
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
			//Renderer2D::drawRect(t->position, t->scale, t->rotation, component->sprite, component->color);
		}

		Renderer2D::endScene();

		UIRenderer::startScene();

		for (auto [entity, component] : View<UIRendererComponent>())
		{
			auto* c = GetComponent<UIConstraintsComponent>(entity);

			Sprite* sprite = &component->sprite;
			Color* color = &component->color;
			Color* borderColor = &component->color;
			float borderWeight = 0;
			float edgeRadius = 0;

			if (HasComponent<UIHoverComponent>(entity))
			{
				auto* h = GetComponent<UIHoverComponent>(entity);
				if (h->hovered)
				{
					sprite = &h->sprite;
					color = &h->color;
				}
			}

			if (HasComponent<UIBorderComponent>(entity))
			{
				auto* b = GetComponent<UIBorderComponent>(entity);
				borderColor = &b->color;
				borderWeight = b->weight;
			}

			if (HasComponent<UIRoundedEdgeComponent>(entity))
			{
				auto* r = GetComponent<UIRoundedEdgeComponent>(entity);
				edgeRadius = r->edgeRadius;
			}

			//UIRenderer::drawElement(c->x, c->y, c->zIndex, c->width, c->height, *sprite, *color, borderWeight, *borderColor, edgeRadius);
		}

		FontRenderer::startScene();
		//FontRenderer::drawString(100, 100, 0.0f, "Calibri", *font1, Colors::Black);
		FontRenderer::drawString(0, 0, 0.0f, "Arial", *font, Colors::Cyan);
		FontRenderer::endScene();

		UIRenderer::endScene();
	}

	void Scene::OnMouseButtonReleased(const MouseButtonReleasedEvent& e)
	{
		for (auto [entity, component] : View<UIClickListenerComponent>())
		{
			auto* c = GetComponent<UIConstraintsComponent>(entity);
			if (e.x >= c->x && e.x < c->x + c->width && e.y >= c->y && e.y < c->y + c->width)
				component->listener(e);
		}
	}

	void Scene::OnMouseMoved(const MouseMovedEvent& e)
	{
		for (auto [entity, component] : View<UIHoverComponent>())
		{
			auto* c = GetComponent<UIConstraintsComponent>(entity);
			GetComponent<UIHoverComponent>(entity)->hovered = (e.x >= c->x && e.x < c->x + c->width && e.y >= c->y && e.y < c->y + c->width);
		}
	}

}
