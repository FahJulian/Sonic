#pragma once
#include <cmath>
#include <algorithm>
#include "Sonic/Debug/Profiler/Profiler.h"
#include "Sonic/Renderer/Renderer2D.h"
#include "Sonic/Graphics/Font/FontRenderer.h"
#include "Sonic/Graphics/Font/Font.h"
#include "Sonic/UI/UI.h"
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

		AddListener(this, &Scene::OnUIComponentAdded);
		AddListener(this, &Scene::OnUIComponentRemoved);
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
		SONIC_PROFILE_FUNCTION("Scene::Update");

		OnUpdate(deltaTime);
		UpdateComponents(deltaTime);
		UpdateBehaviours(deltaTime);
		UpdatePools();
		PollCollisionEvents();

		Window::setCursor(Cursors::Crosshair);

		Draw();
	}

	void Scene::UpdatePools()
	{
		SONIC_PROFILE_FUNCTION("Scene::UpdatePools");

		for (BaseBehaviourPool* pool : m_BehaviourPools)
			pool->UpdatePool();

		for (BaseComponentPool* pool : m_ComponentPools)
			pool->OnUpdate();
	}

	void Scene::UpdateBehaviours(float deltaTime)
	{
		SONIC_PROFILE_FUNCTION("Scene::UpdateBehaviours");

		for (BaseBehaviourPool* pool : m_BehaviourPools)
			pool->UpdateBehaviours(deltaTime);
	}

	void Scene::UpdateComponents(float deltaTime)
	{
		SONIC_PROFILE_FUNCTION("Scene::UpdateComponents");

		for (auto component : ViewComponents<UIComponent>())
			if (component->IsDirty())
				component->Recalculate(this);

		for (auto [entity, cameraComponent, t] : Group<Camera2DComponent, Transform2DComponent>())
		{
			cameraComponent->camera.SetPosition(t->position);
			cameraComponent->camera.SetRotation(t->rotation);

			if (cameraComponent->isSceneCamera)
				m_Camera = cameraComponent->camera;
		}
	}

	void Scene::RenderUIEntity(EntityID entity, Sprite* sprite, Color* color, float x, float y, float zIndex, float width, float height)
	{
		SONIC_PROFILE_FUNCTION("Scene::RenderUIEntity");

		Color* borderColor = color;
		float borderWeight = 0;
		float edgeRadius = 0;

		if (HasComponent<UIHoverComponent>(entity))
		{
			auto* h = GetComponent<UIHoverComponent>(entity);

			if (HasComponent<ResizableComponent>(entity))
			{
				auto* r = GetComponent<ResizableComponent>(entity);
				if (r->dragged.bottom || r->dragged.top || r->dragged.right || r->dragged.left)
					h->hovered = true;
			}

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

		if (HasComponent<TextComponent>(entity))
		{
			auto* t = GetComponent<TextComponent>(entity);
			int textWidth = t->font.StringWidth(t->text);
			int textHeight = t->font.StringHeight(t->text);

			FontRenderer::drawString(x + (width - textWidth) / 2, y + (height - textHeight) / 2, zIndex, t->text, t->font, t->color);
		}

		UIRenderer::drawElement(x, y, zIndex, width, height, *sprite, *color, borderWeight, *borderColor, edgeRadius);
	}

	void Scene::Draw()
	{
		SONIC_PROFILE_FUNCTION("Scene::Draw");

		Renderer2D::startScene(&m_Camera);
		UIRenderer::startScene();
		FontRenderer::startScene();

		for (auto [e, r, t] : Group<Renderer2DComponent, Transform2DComponent>())
			Renderer2D::drawRect(t->position, t->scale, t->rotation, r->sprite, r->color);

		for (auto [e, r] : View<UIRendererComponent>())
		{
			auto* c = GetComponent<UIComponent>(e);
			RenderUIEntity(e, &r->sprite, &r->color, c->GetX(), c->GetY(), c->GetZIndex(), c->GetWidth(), c->GetHeight());
		}
	}

	void Scene::Render()
	{
		SONIC_PROFILE_FUNCTION("Scene::Render");

		Renderer2D::endScene();
		UIRenderer::endScene();
		FontRenderer::endScene();
	}

	void Scene::OnMouseButtonPressed(const MouseButtonPressedEvent& e)
	{
		SONIC_PROFILE_FUNCTION("Scene::OnMouseButtonPressed");

		if (e.button != Buttons::Left)
			return;

		for (auto [entity, r, c] : Group<ResizableComponent, UIComponent>())
		{
			float x = c->GetX();
			float y = c->GetY();
			float width = c->GetWidth();
			float height = c->GetHeight();

			if (r->resizable.bottom && e.x >= x - r->grabSize && e.x < x + width + r->grabSize && e.y >= y - r->grabSize && e.y < y + r->grabSize)
			{
				r->dragged.bottom = true;
				m_ResizingUIEntity = entity;
			}
			else if (r->resizable.top && e.x >= x - r->grabSize && e.x < x + width + r->grabSize && e.y >= y + height - r->grabSize && e.y < y + height + r->grabSize)
			{
				r->dragged.top = true;
				m_ResizingUIEntity = entity;
			}
			
			if (r->resizable.right && e.x >= x + width - r->grabSize && e.x < x + width + r->grabSize && e.y >= y - r->grabSize && e.y < y + height + r->grabSize)
			{
				r->dragged.right = true;
				m_ResizingUIEntity = entity;

			}
			else if (r->resizable.left && e.x >= x - r->grabSize && e.x < x + r->grabSize && e.y >= y - r->grabSize && e.y < y + height + r->grabSize)
			{
				r->dragged.left = true;
				m_ResizingUIEntity = entity;
			}

			if (m_ResizingUIEntity != 0)
				break;
		}
	}

	void Scene::OnMouseButtonReleased(const MouseButtonReleasedEvent& e)
	{
		SONIC_PROFILE_FUNCTION("Scene::OnMouseButtonReleased");

		if (e.button != Buttons::Left)
			return;

		for (auto [entity, clickListener, c] : Group<UIClickListenerComponent, UIComponent>())
		{
			float x = c->GetX();
			float y = c->GetY();
			float width = c->GetWidth();
			float height = c->GetHeight();

			if (HasComponent<ResizableComponent>(entity))
			{
				auto* r = GetComponent<ResizableComponent>(entity);
				if (r->dragged.bottom || r->dragged.top || r->dragged.right || r->dragged.left)
					continue;
			}

			if (e.x >= x && e.x < x + width && e.y >= y && e.y < y + height)
				clickListener->listener(e);
		}
		
		for (auto c : ViewComponents<ResizableComponent>())
		{
			c->dragged.bottom = false;
			c->dragged.right = false;
			c->dragged.top = false;
			c->dragged.left = false;
		}

		m_ResizingUIEntity = 0;
	}

	void Scene::OnMouseMoved(const MouseMovedEvent& e)
	{
		SONIC_PROFILE_FUNCTION("Scene::OnMouseMoved");

		for (auto [entity, h, c] : Group<UIHoverComponent, UIComponent>())
		{
			float x = UI::toAbsoluteX(this, c->parent, c->x);
			float y = UI::toAbsoluteY(this, c->parent, c->y);
			float width = UI::toAbsoluteWidth(this, c->parent, c->width);
			float height = UI::toAbsoluteHeight(this, c->parent, c->height);

			h->hovered = (e.x >= x && e.x < x + width && e.y >= y && e.y < y + height);
		}
	}

	void Scene::OnMouseDragged(const MouseDraggedEvent& e)
	{
		SONIC_PROFILE_FUNCTION("Scene::OnMouseDragged");

		if (e.button != Buttons::Left)
			return;

		if (m_ResizingUIEntity == 0)
			return;

		auto* r = GetComponent<ResizableComponent>(m_ResizingUIEntity);
		auto* c = GetComponent<UIComponent>(m_ResizingUIEntity);

		if (r->dragged.bottom)
		{
			float y = c->GetY();
			float height = c->GetHeight();
			float dy = std::clamp<float>(y - e.y, UI::toAbsoluteHeight(this, c->parent, r->maxHeight) - height, UI::toAbsoluteHeight(this, c->parent, r->maxHeight) - height);

			if (c->y.mode == UISize::Mode::Absolute)
				c->y.value -= dy;
			else if (c->y.mode == UISize::Mode::RelativeToEntity)
				c->y.value -= dy / height;
			else if (c->y.mode == UISize::Mode::RelativeToWindow)
				c->y.value -= dy / Window::getHeight();

			if (c->height.mode == UISize::Mode::Absolute)
				c->height.value += dy;
			else if (c->height.mode == UISize::Mode::RelativeToEntity)
				c->height.value += dy / height;
			else if (c->height.mode == UISize::Mode::RelativeToWindow)
				c->height.value += dy / Window::getHeight();
		}
		else if (r->dragged.top)
		{
			float y = c->GetY();
			float height = c->GetHeight();
			float dy = std::clamp<float>(e.y - (y + height), UI::toAbsoluteHeight(this, c->parent, r->minHeight) - height, UI::toAbsoluteHeight(this, c->parent, r->maxHeight) - height);

			if (c->height.mode == UISize::Mode::Absolute)
				c->height.value += dy;
			else if (c->height.mode == UISize::Mode::RelativeToEntity)
				c->height.value += dy / height;
			else if (c->height.mode == UISize::Mode::RelativeToWindow)
				c->height.value += dy / Window::getHeight();
		}

		if (r->dragged.right)
		{
			float x = c->GetX();
			float width = c->GetWidth();
			float dx = std::clamp<float>(e.x - (x + width), UI::toAbsoluteWidth(this, c->parent, r->minWidth) - width, UI::toAbsoluteWidth(this, c->parent, r->maxWidth) - width);

			if (c->width.mode == UISize::Mode::Absolute)
				c->width.value += dx;
			else if (c->width.mode == UISize::Mode::RelativeToEntity)
				c->width.value += dx / width;
			else if (c->width.mode == UISize::Mode::RelativeToWindow)
				c->width.value += dx / Window::getWidth();
		} 
		else if (r->dragged.left)
		{
			float x = c->GetX();
			float width = c->GetWidth();
			float dx = std::clamp<float>(x - e.x, UI::toAbsoluteWidth(this, c->parent, r->minWidth) - width, UI::toAbsoluteWidth(this, c->parent, r->maxWidth) - width);

			if (c->x.mode == UISize::Mode::Absolute)
				c->x.value -= dx;
			else if (c->x.mode == UISize::Mode::RelativeToEntity)
				c->x.value -= dx / width;
			else if (c->x.mode == UISize::Mode::RelativeToWindow)
				c->x.value -= dx / Window::getWidth();

			if (c->width.mode == UISize::Mode::Absolute)
				c->width.value += dx;
			else if (c->width.mode == UISize::Mode::RelativeToEntity)
				c->width.value += dx / width;
			else if (c->width.mode == UISize::Mode::RelativeToWindow)
				c->width.value += dx / Window::getWidth();
		}
	}
	
	void Scene::OnUIComponentAdded(const ComponentAddedEvent<UIComponent>& e)
	{
		std::cout << "UI Component added" << std::endl;
	}

	void Scene::OnUIComponentRemoved(const ComponentRemovedEvent<UIComponent>& e)
	{
		std::cout << "UI Component removed" << std::endl;
	}
}
