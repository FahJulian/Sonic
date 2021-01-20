#pragma once
#include <algorithm>
#include "Sonic/Renderer/Renderer2D.h"
#include "Sonic/Graphics/Font/FontRenderer.h"
#include "Sonic/Graphics/Font/Font.h"
#include "Sonic/UI/UIComponents.h"
#include "Sonic/UI/UIRenderer.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"


namespace Sonic {

	Scene::Scene()
		: m_Camera(Camera2D(0, Window::getWidth(), 0, Window::getHeight())) 
	{
		AddListener(this, &Scene::OnMouseButtonReleased);
		AddListener(this, &Scene::OnMouseButtonPressed);
		AddListener(this, &Scene::OnMouseMoved);
		AddListener(this, &Scene::OnMouseDragged);
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
		for (auto [entity, cameraComponent, t] : Group<Camera2DComponent, Transform2DComponent>())
		{
			cameraComponent->camera.SetPosition(t->position);
			cameraComponent->camera.SetRotation(t->rotation);

			if (cameraComponent->isSceneCamera)
				m_Camera = cameraComponent->camera;
		}
	}

	void Scene::Render()
	{
		Renderer2D::startScene(&m_Camera);
		UIRenderer::startScene();
		FontRenderer::startScene();

		for (auto [e, r, t] : Group<Renderer2DComponent, Transform2DComponent>())
			Renderer2D::drawRect(t->position, t->scale, t->rotation, r->sprite, r->color);

		for (auto [e, r, c] : Group<UIRendererComponent, UIConstraintsComponent>())
		{
			Sprite* sprite = &r->sprite;
			Color* color = &r->color;
			Color* borderColor = &r->color;
			float borderWeight = 0;
			float edgeRadius = 0;

			if (HasComponent<UIHoverComponent>(e))
			{
				auto* h = GetComponent<UIHoverComponent>(e);

				if (HasComponent<ResizableComponent>(e))
				{
					auto* r = GetComponent<ResizableComponent>(e);
					for (int i = 0; i < 8; i++)
						if (r->dragged[i])
							h->hovered = true;
				}

				if (h->hovered)
				{
					sprite = &h->sprite;
					color = &h->color;
				}
			}

			if (HasComponent<UIBorderComponent>(e))
			{
				auto* b = GetComponent<UIBorderComponent>(e);
				borderColor = &b->color;
				borderWeight = b->weight;
			}

			if (HasComponent<UIRoundedEdgeComponent>(e))
			{
				auto* r = GetComponent<UIRoundedEdgeComponent>(e);
				edgeRadius = r->edgeRadius;
			}

			if (HasComponent<TextComponent>(e))
			{
				auto* t = GetComponent<TextComponent>(e);
				int width = t->font.StringWidth(t->text);
				int height = t->font.StringHeight(t->text);

				FontRenderer::drawString(c->x + (c->width - width) / 2, c->y + (c->height - height) / 2, c->zIndex, t->text, t->font, t->color);
			}

			UIRenderer::drawElement(c->x, c->y, c->zIndex, c->width, c->height, *sprite, *color, borderWeight, *borderColor, edgeRadius);
		}

		Renderer2D::endScene();
		UIRenderer::endScene();
		FontRenderer::endScene();
	}

	void Scene::OnMouseButtonPressed(const MouseButtonPressedEvent& e)
	{
		if (e.button != Buttons::Left)
			return;

		for (auto [entity, r, c] : Group<ResizableComponent, UIConstraintsComponent>())
		{
			if (r->edges[0] && e.x >= c->x - r->grabSize && e.x < c->x + c->width + r->grabSize && e.y >= c->y - r->grabSize && e.y < c->y + r->grabSize)
				r->dragged[0] = true;
			else if (r->edges[2] && e.x >= c->x - r->grabSize && e.x < c->x + c->width + r->grabSize && e.y >= c->y + c->height - r->grabSize && e.y < c->y + c->height + r->grabSize)
				r->dragged[2] = true;
			
			if (r->edges[1] && e.x >= c->x + c->width - r->grabSize && e.x < c->x + c->width + r->grabSize && e.y >= c->y - r->grabSize && e.y < c->y + c->height + r->grabSize)
				r->dragged[1] = true;
			else if (r->edges[3] && e.x >= c->x - r->grabSize && e.x < c->x + r->grabSize && e.y >= c->y - r->grabSize && e.y < c->y + c->height + r->grabSize)
				r->dragged[3] = true;
		}
	}

	void Scene::OnMouseButtonReleased(const MouseButtonReleasedEvent& e)
	{
		if (e.button != Buttons::Left)
			return;

		for (auto [entity, clickListener, c] : Group<UIClickListenerComponent, UIConstraintsComponent>())
		{
			if (HasComponent<ResizableComponent>(entity))
			{
				auto* r = GetComponent<ResizableComponent>(entity);
				for (int i = 0; i < 8; i++)
					if (r->dragged[i])
						goto skip;
			}

			if (e.x >= c->x && e.x < c->x + c->width && e.y >= c->y && e.y < c->y + c->height)
				clickListener->listener(e);

		skip:;
		}
		
		for (auto c : ViewComponents<ResizableComponent>())
		{
			c->dragged[0] = false;
			c->dragged[1] = false;
			c->dragged[2] = false;
			c->dragged[3] = false;
		}
	}

	void Scene::OnMouseMoved(const MouseMovedEvent& e)
	{
		for (auto [entity, h, c] : Group<UIHoverComponent, UIConstraintsComponent>())
			h->hovered = (e.x >= c->x && e.x < c->x + c->width && e.y >= c->y && e.y < c->y + c->height);
	}

	void Scene::OnMouseDragged(const MouseDraggedEvent& e)
	{
		if (e.button != Buttons::Left)
			return;

		for (auto [entity, r, c] : Group<ResizableComponent, UIConstraintsComponent>())
		{
			if (r->dragged[0])
			{
				float dy = std::clamp<float>(c->y - e.y, r->minHeight - c->height, r->maxHeight - c->height);
				c->y -= dy;
				c->height += dy;
			}
			else if (r->dragged[2])
			{
				float dy = std::clamp<float>(e.y - (c->y + c->height), r->minHeight - c->height, r->maxHeight - c->height);
				c->height += dy;
			}

			if (r->dragged[1])
			{
				float dx = std::clamp<float>(e.x - (c->x + c->width), r->minWidth - c->width, r->maxWidth - c->width);
				c->width += dx;
			} 
			else if (r->dragged[3])
			{
				float dx = std::clamp<float>(c->x - e.x, r->minWidth - c->width, r->maxWidth - c->width);
				c->x -= dx;
				c->width += dx;
			}
		}
	}

}
