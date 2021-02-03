#pragma once
#include <cmath>
#include <algorithm>
#include "Sonic/Debug/Profiler/Profiler.h"
#include "Sonic/Renderer/Renderer2D.h"
#include "Sonic/UI/Font/FontRenderer.h"
#include "Sonic/UI/UIComponents.h"
#include "Sonic/UI/UIRenderer.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "PairView.h"
#include "ComponentView.h"
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
		AddListener(this, &Scene::OnTextComponentAdded);

		AddListener(this, &Scene::OnRenderer2DComponentAdded);
		AddListener(this, &Scene::OnTransform2DComponentAdded);
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

		Window::pollEvents();

		PollCollisionEvents();

		UpdateRenderers();
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

		for (auto c : ViewComponents<UIComponent>())
		{
			if (c->IsDirty())
			{
				UIComponent* parentComponent = c->parent != 0 ? GetComponent<UIComponent>(c->parent) : nullptr;

				switch (c->x.mode)
				{
				case UISize::Mode::Absolute: c->absoluteX = c->x.value; break;
				case UISize::Mode::RelativeToEntity: c->absoluteX = parentComponent->GetX() + c->x.value * parentComponent->GetWidth(); break;
				case UISize::Mode::RelativeToWindow: c->absoluteX = c->x.value * Window::getWidth(); break;
				}

				switch (c->y.mode)
				{
				case UISize::Mode::Absolute: c->absoluteY = c->y.value; break;
				case UISize::Mode::RelativeToEntity: c->absoluteY = parentComponent->GetY() + c->y.value * parentComponent->GetHeight(); break;
				case UISize::Mode::RelativeToWindow: c->absoluteY = c->y.value * Window::getHeight(); break;
				}

				switch (c->width.mode)
				{
				case UISize::Mode::Absolute: c->absoluteWidth = c->width.value; break;
				case UISize::Mode::RelativeToEntity: c->absoluteWidth = c->width.value * parentComponent->GetWidth(); break;
				case UISize::Mode::RelativeToWindow: c->absoluteWidth = c->width.value * Window::getWidth(); break;
				}

				switch (c->height.mode)
				{
				case UISize::Mode::Absolute: c->absoluteHeight = c->height.value; break;
				case UISize::Mode::RelativeToEntity: c->absoluteHeight = c->height.value * parentComponent->GetHeight(); break;
				case UISize::Mode::RelativeToWindow: c->absoluteHeight = c->height.value * Window::getHeight(); break;
				}

				c->dirty = false;
				c->SetRendererDirty();

				for (EntityID child : *c->childs)
				{
					auto* childComponent = GetComponent<UIComponent>(child);
					if (!childComponent->IsDirty())
					{
						if (childComponent->x.mode == UISize::Mode::RelativeToEntity) childComponent->absoluteX = c->absoluteX + childComponent->x.value * c->absoluteWidth;
						if (childComponent->y.mode == UISize::Mode::RelativeToEntity) childComponent->absoluteY = c->absoluteY + childComponent->y.value * c->absoluteHeight;
						if (childComponent->width.mode == UISize::Mode::RelativeToEntity) childComponent->absoluteWidth = childComponent->width.value * c->absoluteWidth;
						if (childComponent->height.mode == UISize::Mode::RelativeToEntity) childComponent->absoluteHeight = childComponent->height.value * c->absoluteHeight;
						childComponent->SetRendererDirty();
					}
				}
			}
		}

		for (auto [entity, cameraComponent, t] : Group<Camera2DComponent, Transform2DComponent>())
		{
			cameraComponent->camera.SetPosition(t->GetPosition());
			cameraComponent->camera.SetRotation(t->GetRotation());

			if (cameraComponent->isSceneCamera)
				m_Camera = cameraComponent->camera;
		}
	}

	void Scene::UpdateRenderers()
	{
		SONIC_PROFILE_FUNCTION("Scene::UpdateRenderers");

		Renderer2D::update(this, &m_Camera);
		UIRenderer::update(this);
		FontRenderer::update(this);
	}

	void Scene::Render()
	{
		SONIC_PROFILE_FUNCTION("Scene::Render");

		Renderer2D::render();
		UIRenderer::render();
		FontRenderer::render();
	}

	void Scene::OnMouseButtonPressed(const MouseButtonPressedEvent& e)
	{
		SONIC_PROFILE_FUNCTION("Scene::OnMouseButtonPressed");

		if (e.button != Buttons::Left)
			return;

		for (auto [entity, r, c] : Group<UIResizableComponent, UIComponent>())
		{
			r->dragged.bottom = r->hovered.bottom;
			r->dragged.top = r->hovered.top && !r->hovered.bottom;
			r->dragged.left = r->hovered.left;
			r->dragged.right = r->hovered.right && !r->hovered.left;

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

			if (HasComponent<UIResizableComponent>(entity))
			{
				auto* r = GetComponent<UIResizableComponent>(entity);
				if (r->dragged.bottom || r->dragged.top || r->dragged.right || r->dragged.left)
					continue;
			}

			if (e.x >= x && e.x < x + width && e.y >= y && e.y < y + height)
				clickListener->listener(e);
		}
		
		for (auto c : ViewComponents<UIResizableComponent>())
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

		Window::setCursor(StandardCursors::Arrow);

		for (auto [entity, h, c] : Group<UIHoverComponent, UIComponent>())
			h->SetHoverered(e.x >= c->GetX() && e.x < c->GetX() + c->GetWidth() && e.y >= c->GetY() && e.y < c->GetY() + c->GetHeight());

		for (auto [entity, r, c] : Group<UIResizableComponent, UIComponent>())
		{
			r->hovered = { false, false, false, false };

			if (r->resizable.bottom && e.x >= c->GetX() - r->grabSize && e.x < c->GetX() + c->GetWidth() + r->grabSize && e.y >= c->GetY() - r->grabSize && e.y < c->GetY() + r->grabSize)
				r->hovered.bottom = true;
			else if (r->resizable.top && e.x >= c->GetX() - r->grabSize && e.x < c->GetX() + c->GetWidth() + r->grabSize && e.y >= c->GetY() + c->GetHeight() - r->grabSize && e.y < c->GetY() + c->GetHeight() + r->grabSize)
				r->hovered.top = true;

			if (r->resizable.right && e.x >= c->GetX() + c->GetWidth() - r->grabSize && e.x < c->GetX() + c->GetWidth() + r->grabSize && e.y >= c->GetY() - r->grabSize && e.y < c->GetY() + c->GetHeight() + r->grabSize)
				r->hovered.right = true;
			else if (r->resizable.left && e.x >= c->GetX() - r->grabSize && e.x < c->GetX() + r->grabSize && e.y >= c->GetY() - r->grabSize && e.y < c->GetY() + c->GetHeight() + r->grabSize)
				r->hovered.left = true;

			if ((r->dragged.bottom && r->dragged.right) || (r->dragged.top && r->dragged.left))
			{
				Window::setCursor(StandardCursors::ResizeDiagonalLeft);
				continue;
			}
			else if ((r->dragged.bottom && r->dragged.left) || (r->dragged.top && r->dragged.right))
			{
				Window::setCursor(StandardCursors::ResizeDiagonalRight);
				continue;
			}
			else if (r->dragged.bottom || r->dragged.top)
			{
				Window::setCursor(StandardCursors::ResizeVertical);
				continue;
			}
			else if (r->dragged.left || r->dragged.right)
			{
				Window::setCursor(StandardCursors::ResizeHorizontal);
				continue;
			}

			if ((r->hovered.bottom && r->hovered.right) || (r->hovered.top && r->hovered.left))
				Window::setCursor(StandardCursors::ResizeDiagonalLeft);
			else if ((r->hovered.bottom && r->hovered.left) || (r->hovered.top && r->hovered.right))
				Window::setCursor(StandardCursors::ResizeDiagonalRight);
			else if (r->hovered.bottom || r->hovered.top)
				Window::setCursor(StandardCursors::ResizeVertical);
			else if (r->hovered.left || r->hovered.right)
				Window::setCursor(StandardCursors::ResizeHorizontal);
		}
	}

	void Scene::OnMouseDragged(const MouseDraggedEvent& e)
	{
		SONIC_PROFILE_FUNCTION("Scene::OnMouseDragged");

		if (e.button != Buttons::Left)
			return;

		for (auto [entity, r, c] : Group<UIResizableComponent, UIComponent>())
		{
			UIComponent* pc = c->parent != 0 ? pc = GetComponent<UIComponent>(c->parent) : nullptr;

			if (r->dragged.bottom)
			{
				float dy = std::clamp<float>(c->GetY() - e.y, UI::toAbsoluteHeight(this, c->parent, r->minHeight) - c->GetHeight(), UI::toAbsoluteHeight(this, c->parent, r->maxHeight) - c->GetHeight());

				c->SetY(c->GetY() + dy);
				c->SetHeight(c->GetHeight() + dy);


				//if (c->y.mode == UISize::Mode::Absolute)
				//	c->SetY(c->GetY() - dy);
				//else if (c->y.mode == UISize::Mode::RelativeToEntity)
				//	c->SetY(c->y.value - dy / pc->GetHeight());
				//else if (c->y.mode == UISize::Mode::RelativeToWindow)
				//	c->SetY(c->y.value - dy / Window::getHeight());

				//if (c->height.mode == UISize::Mode::Absolute)
				//	c->SetHeight(c->GetHeight() + dy);
				//else if (c->height.mode == UISize::Mode::RelativeToEntity)
				//	c->SetHeight(c->height.value + dy / pc->GetHeight());
				//else if (c->height.mode == UISize::Mode::RelativeToWindow)
				//	c->SetHeight(c->height.value + dy / Window::getHeight());
			}
			else if (r->dragged.top)
			{
				float dy = std::clamp<float>(e.y - (c->GetY() + c->GetHeight()), UI::toAbsoluteHeight(this, c->parent, r->minHeight) - c->GetHeight(), UI::toAbsoluteHeight(this, c->parent, r->maxHeight) - c->GetHeight());

				c->SetHeight(c->GetHeight() + dy);
				//if (c->height.mode == UISize::Mode::Absolute)
				//	c->SetHeight(c->GetHeight() + dy);
				//else if (c->height.mode == UISize::Mode::RelativeToEntity)
				//	c->SetHeight(c->height.value + dy / pc->GetHeight());
				//else if (c->height.mode == UISize::Mode::RelativeToWindow)
				//	c->SetHeight(c->height.value + dy / Window::getHeight());
			}

			if (r->dragged.right)
			{
				float dx = std::clamp<float>(e.x - (c->GetX() + c->GetWidth()), UI::toAbsoluteWidth(this, c->parent, r->minWidth) - c->GetWidth(), UI::toAbsoluteWidth(this, c->parent, r->maxWidth) - c->GetWidth());
				std::cout << dx << std::endl;
				c->SetWidth(c->GetWidth() + dx);
				//if (c->width.mode == UISize::Mode::Absolute)
				//	c->SetWidth(c->GetWidth() + dx);
				//else if (c->width.mode == UISize::Mode::RelativeToEntity)
				//	c->SetWidth(c->width.value + dx / pc->GetWidth());
				//else if (c->width.mode == UISize::Mode::RelativeToWindow)
				//	c->SetWidth(c->width.value + dx / Window::getWidth());
			}
			else if (r->dragged.left)
			{
				float dx = std::clamp<float>(c->GetX() - e.x, UI::toAbsoluteWidth(this, c->parent, r->minWidth) - c->GetWidth(), UI::toAbsoluteWidth(this, c->parent, r->maxWidth) - c->GetWidth());
				std::cout << dx << std::endl;
				c->SetX(c->GetX() + dx);
				c->SetWidth(c->GetWidth() + dx);

				std::cout << std::endl << c->x.value << std::endl;
				std::cout << c->width.value << std::endl << std::endl;

				//if (c->x.mode == UISize::Mode::Absolute)
				//	c->SetX(c->GetX() - dx);
				//else if (c->x.mode == UISize::Mode::RelativeToEntity)
				//	c->SetX(c->x.value - dx / pc->GetWidth());
				//else if (c->x.mode == UISize::Mode::RelativeToWindow)
				//	c->SetX(c->x.value + dx / Window::getWidth());

				//if (c->width.mode == UISize::Mode::Absolute)
				//	c->SetWidth(c->GetWidth() + dx);
				//else if (c->width.mode == UISize::Mode::RelativeToEntity)
				//	c->SetWidth(c->width.value + dx / pc->GetWidth());
				//else if (c->width.mode == UISize::Mode::RelativeToWindow)
				//	c->SetWidth(c->width.value + dx / Window::getWidth());
			}
		}

	}
	
	void Scene::OnUIComponentAdded(const ComponentAddedEvent<UIComponent>& e)
	{
		UIComponent* c = GetComponent<UIComponent>(e.entity);

		if (HasComponent<UIRendererComponent>(e.entity))
			c->uiRendererDirty = GetComponent<UIRendererComponent>(e.entity)->dirty;
		if (HasComponent<TextComponent>(e.entity))
			c->fontRendererDirty = GetComponent<TextComponent>(e.entity)->dirty;

		if (c->parent != 0)
			GetComponent<UIComponent>(c->parent)->childs->push_back(e.entity);
	}

	void Scene::OnUIHoverComponentAdded(const ComponentAddedEvent<UIHoverComponent>& e)
	{
		if (HasComponent<UIRendererComponent>(e.entity))
			GetComponent<UIHoverComponent>(e.entity)->uiRendererDirty = GetComponent<UIRendererComponent>(e.entity)->dirty;
	}

	void Scene::OnUIRendererComponentAdded(const ComponentAddedEvent<UIRendererComponent>& e)
	{
		if (HasComponent<UIComponent>(e.entity))
			GetComponent<UIComponent>(e.entity)->uiRendererDirty = GetComponent<UIRendererComponent>(e.entity)->dirty;
		if (HasComponent<UIHoverComponent>(e.entity))
			GetComponent<UIHoverComponent>(e.entity)->uiRendererDirty = GetComponent<UIRendererComponent>(e.entity)->dirty;
	}

	void Scene::OnTextComponentAdded(const ComponentAddedEvent<TextComponent>& e) 
	{
		if (HasComponent<UIComponent>(e.entity))
			GetComponent<UIComponent>(e.entity)->fontRendererDirty = GetComponent<TextComponent>(e.entity)->dirty;
	}

	void Scene::OnRenderer2DComponentAdded(const ComponentAddedEvent<Renderer2DComponent>& e)
	{
		if (HasComponent<Transform2DComponent>(e.entity))
			GetComponent<Transform2DComponent>(e.entity)->rendererDirty = GetComponent<Renderer2DComponent>(e.entity)->dirty;
	}

	void Scene::OnTransform2DComponentAdded(const ComponentAddedEvent<Transform2DComponent>& e)
	{
		if (HasComponent<Renderer2DComponent>(e.entity))
			GetComponent<Transform2DComponent>(e.entity)->rendererDirty = GetComponent<Renderer2DComponent>(e.entity)->dirty;
	}

	void Scene::OnUIComponentRemoved(const ComponentRemovedEvent<UIComponent>& e)
	{
		auto* c = GetComponent<UIComponent>(e.entity);
		if (c->parent != 0)
		{
			auto* parentComponent = GetComponent<UIComponent>(c->parent);
			parentComponent->childs->erase(std::remove(parentComponent->childs->begin(), parentComponent->childs->end(), e.entity));
		}
	}
}
