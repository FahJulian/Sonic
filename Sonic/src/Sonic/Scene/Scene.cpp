#pragma once
#include <cmath>
#include <algorithm>
#include "Sonic/Debug/Profiler/Profiler.h"
#include "Sonic/Renderer/Renderer2D.h"
#include "Sonic/UI/Font/FontRenderer.h"
#include "Sonic/UI/Font/Font.h"
#include "Sonic/UI/UI.h"
#include "Sonic/UI/UIComponents.h"
#include "Sonic/UI/UIRenderer.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "PairView.h"
#include "ComponentView.h"
#include "EntityView.h"
#include "GroupView.h"


namespace Sonic {

	Scene::Scene()
		: m_Camera(Camera2D(0, Window::getWidth(), 0, Window::getHeight())) 
	{
		AddListener(this, &Scene::OnMouseButtonReleased);
		AddListener(this, &Scene::OnMouseButtonPressed);
		AddListener(this, &Scene::OnMouseMoved);
		AddListener(this, &Scene::OnMouseDragged);

AddListener(this, &Scene::OnUIComponentAdded);
AddListener(this, &Scene::OnUIHoverComponentAdded);
AddListener(this, &Scene::OnUIRendererComponentAdded);
	}

	Entity Scene::AddEntity()
	{
		return Entity(this);
	}

	void Scene::RemoveEntity(EntityID entity)
	{
		for (BaseBehaviourPool* pool : GenericContainer::GetAll<BaseBehaviourPool>(this))
			if (pool->HasEntity(entity))
				pool->RemoveEntity(entity);

		for (BaseComponentPool* pool : GenericContainer::GetAll<BaseComponentPool>(this))
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

		//Window::setCursor(Cursors::Crosshair);

		Draw();
	}

	void Scene::UpdatePools()
	{
		SONIC_PROFILE_FUNCTION("Scene::UpdatePools");

		for (BaseBehaviourPool* pool : GenericContainer::GetAll<BaseBehaviourPool>(this))
			pool->UpdatePool();

		for (BaseComponentPool* pool : GenericContainer::GetAll<BaseComponentPool>(this))
			pool->OnUpdate();
	}

	void Scene::UpdateBehaviours(float deltaTime)
	{
		SONIC_PROFILE_FUNCTION("Scene::UpdateBehaviours");

		for (BaseBehaviourPool* pool : GenericContainer::GetAll<BaseBehaviourPool>(this))
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

	void Scene::Draw()
	{
		SONIC_PROFILE_FUNCTION("Scene::Draw");

		Renderer2D::startScene(&m_Camera);
		FontRenderer::startScene();

		for (auto [e, r, t] : Group<Renderer2DComponent, Transform2DComponent>())
			Renderer2D::drawRect(t->position, t->scale, t->rotation, r->sprite, r->color);

		UIRenderer::update(this);

		for (auto [e, t, c] : Group<TextComponent, UIComponent>())
		{
			int textWidth = t->font.StringWidth(t->text);
			int textHeight = t->font.StringHeight(t->text);

			FontRenderer::drawString(c->GetX() + (c->GetWidth() - textWidth) / 2, c->GetY() + (c->GetHeight() - textHeight) / 2, c->GetZIndex(), t->text, t->font, t->color);
		}
	}

	void Scene::Render()
	{
		SONIC_PROFILE_FUNCTION("Scene::Render");

		Renderer2D::endScene();
		UIRenderer::render();
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

			h->SetHoverered(e.x >= x && e.x < x + width && e.y >= y && e.y < y + height);
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
		if (HasComponent<UIRendererComponent>(e.entity))
			GetComponent<UIComponent>(e.entity)->rendererDirty = GetComponent<UIRendererComponent>(e.entity)->dirty;
	}

	void Scene::OnUIHoverComponentAdded(const ComponentAddedEvent<UIHoverComponent>& e)
	{
		if (HasComponent<UIRendererComponent>(e.entity))
			GetComponent<UIHoverComponent>(e.entity)->rendererDirty = GetComponent<UIRendererComponent>(e.entity)->dirty;
	}

	void Scene::OnUIRendererComponentAdded(const ComponentAddedEvent<UIRendererComponent>&e)
	{
		if (HasComponent<UIComponent>(e.entity))
			GetComponent<UIComponent>(e.entity)->rendererDirty = GetComponent<UIRendererComponent>(e.entity)->dirty;
		if (HasComponent<UIHoverComponent>(e.entity))
			GetComponent<UIHoverComponent>(e.entity)->rendererDirty = GetComponent<UIRendererComponent>(e.entity)->dirty;
	}
}
