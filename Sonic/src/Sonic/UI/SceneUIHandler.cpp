#include <algorithm>
#include "Sonic/Log/Log.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Window/Input/Mouse.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/Views.h"
#include "UIComponents.h"
#include "SceneUIHandler.h"

using namespace Sonic;


SceneUIHandler::SceneUIHandler(Scene* scene)
	: m_Scene(scene)
{
	m_Scene->AddListener(this, &SceneUIHandler::OnMouseButtonPressed);
	m_Scene->AddListener(this, &SceneUIHandler::OnMouseButtonReleased);
	m_Scene->AddListener(this, &SceneUIHandler::OnMouseMoved);
	m_Scene->AddListener(this, &SceneUIHandler::OnWindowResized);

	m_Scene->AddListener<SceneUIHandler, ComponentAddedEvent<UIComponent>>(this, &SceneUIHandler::OnComponentAdded);
	m_Scene->AddListener<SceneUIHandler, ComponentAddedEvent<UIHoverComponent>>(this, &SceneUIHandler::OnComponentAdded);
	m_Scene->AddListener<SceneUIHandler, ComponentAddedEvent<UITextComponent>>(this, &SceneUIHandler::OnComponentAdded);
	m_Scene->AddListener<SceneUIHandler, ComponentAddedEvent<UIRendererComponent>>(this, &SceneUIHandler::OnComponentAdded);
	m_Scene->AddListener<SceneUIHandler, ComponentRemovedEvent<UIComponent>>(this, &SceneUIHandler::OnComponentRemoved);
}

void SceneUIHandler::Update(float deltaTime)
{
	for (auto c : m_Scene->ViewComponents<UIComponent>())
	{
		if (c->IsDirty())
		{
			UIComponent* parentComponent = c->parent != 0 ? m_Scene->GetComponent<UIComponent>(c->parent) : nullptr;

			c->x.absoluteValue = CalculateAbsoluteX(c->x, parentComponent);
			c->y.absoluteValue = CalculateAbsoluteY(c->y, parentComponent);
			c->width.absoluteValue = CalculateAbsoluteWidth(c->width, parentComponent);
			c->height.absoluteValue = CalculateAbsoluteHeight(c->height, parentComponent);

			c->dirty = false;
			c->SetRendererDirty();

			ResizeChilds(c);
		}
	}

	for (auto [e, r] : m_Scene->View<UIResizableComponent>())
	{
		if (r->IsDirty())
		{
			UIComponent* parentComponent = m_Scene->GetComponent<UIComponent>(e);

			r->minWidth.absoluteValue = CalculateAbsoluteWidth(r->minWidth, parentComponent);
			r->maxWidth.absoluteValue = CalculateAbsoluteWidth(r->maxWidth, parentComponent);
			r->minHeight.absoluteValue = CalculateAbsoluteHeight(r->minHeight, parentComponent);
			r->maxHeight.absoluteValue = CalculateAbsoluteHeight(r->maxHeight, parentComponent);
		}
	}
}

void SceneUIHandler::OnMouseButtonPressed(const MouseButtonPressedEvent& e)
{
	if (e.button == Buttons::Left)
	{
		for (auto* r : m_Scene->ViewComponents<UIResizableComponent>())
		{
			r->dragged.bottom = r->hovered.bottom;
			r->dragged.top = r->hovered.top && !r->hovered.bottom;
			r->dragged.left = r->hovered.left;
			r->dragged.right = r->hovered.right && !r->hovered.left;
		}
	}
}

void SceneUIHandler::OnMouseButtonReleased(const MouseButtonReleasedEvent& e)
{
	for (auto [entity, clickListener, c] : m_Scene->Group<UIClickListenerComponent, UIComponent>())
	{
		if (m_Scene->HasComponent<UIResizableComponent>(entity))
		{
			auto* r = m_Scene->GetComponent<UIResizableComponent>(entity);
			if (r->dragged.bottom || r->dragged.top || r->dragged.right || r->dragged.left)
				continue;
		}

		if (e.x >= c->GetX() && e.x < c->GetX() + c->GetWidth() && e.y >=c ->GetY() && e.y <c ->GetY() + c->GetHeight())
			clickListener->listener(e);
	}

	if (e.button == Buttons::Left)
	{
		for (auto* r : m_Scene->ViewComponents<UIResizableComponent>())
			r->dragged = { false, false, false, false };
	}
}

void SceneUIHandler::OnMouseMoved(const MouseMovedEvent& e)
{
	Window::setCursor(StandardCursors::Arrow);

	for (auto [entity, h, c] : m_Scene->Group<UIHoverComponent, UIComponent>())
			h->SetHoverered(e.x >= c->GetX() && e.x < c->GetX() + c->GetWidth() && e.y >= c->GetY() && e.y < c->GetY() + c->GetHeight());

	for (auto [entity, r, c] : m_Scene->Group<UIResizableComponent, UIComponent>())
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

		if (m_Scene->HasComponent<UIHoverComponent>(entity) && (r->dragged.bottom || r->dragged.top || r->dragged.right || r->dragged.left))
			m_Scene->GetComponent<UIHoverComponent>(entity)->SetHoverered(true);

		if ((r->dragged.bottom && r->dragged.right) || (r->dragged.top && r->dragged.left) || (r->hovered.bottom && r->hovered.right) || (r->hovered.top && r->hovered.left))
			Window::setCursor(StandardCursors::ResizeDiagonalLeft);
		else if ((r->dragged.bottom && r->dragged.left) || (r->dragged.top && r->dragged.right) || (r->hovered.bottom && r->hovered.left) || (r->hovered.top && r->hovered.right))
			Window::setCursor(StandardCursors::ResizeDiagonalRight);
		else if (r->dragged.bottom || r->dragged.top || r->hovered.bottom || r->hovered.top)
			Window::setCursor(StandardCursors::ResizeVertical);
		else if (r->dragged.left || r->dragged.right || r->hovered.left || r->hovered.right)
			Window::setCursor(StandardCursors::ResizeHorizontal);
	}

	if (Mouse::isButtonPressed(Buttons::Left))
	{
		for (auto [entity, r, c] : m_Scene->Group<UIResizableComponent, UIComponent>())
		{
			UIComponent* pc = c->parent != 0 ? pc = m_Scene->GetComponent<UIComponent>(c->parent) : nullptr;

			if (r->dragged.bottom)
			{
				float dy = std::clamp<float>(c->GetY() - e.y, r->minHeight.absoluteValue - c->GetHeight(), r->maxHeight.absoluteValue - c->GetHeight());
				c->SetY(c->GetY() + dy);
				c->SetHeight(c->GetHeight() + dy);
			}
			else if (r->dragged.top)
			{
				float dy = std::clamp<float>(e.y - (c->GetY() + c->GetHeight()), r->minHeight.absoluteValue - c->GetHeight(), r->maxHeight.absoluteValue - c->GetHeight());
				c->SetHeight(c->GetHeight() + dy);
			}

			if (r->dragged.right)
			{
				float dx = std::clamp<float>(e.x - (c->GetX() + c->GetWidth()), r->minWidth.absoluteValue - c->GetWidth(), r->maxWidth.absoluteValue - c->GetWidth());
				c->SetWidth(c->GetWidth() + dx);
			}
			else if (r->dragged.left)
			{
				float dx = std::clamp<float>(c->GetX() - e.x, r->minWidth.absoluteValue - c->GetWidth(), r->maxWidth.absoluteValue - c->GetWidth());
				c->SetX(c->GetX() + dx);
				c->SetWidth(c->GetWidth() + dx);
			}
		}
	}
}

void SceneUIHandler::OnComponentAdded(const ComponentAddedEvent<UIComponent>& e)
{
	UIComponent* c = m_Scene->GetComponent<UIComponent>(e.entity);

	if (m_Scene->HasComponent<UIRendererComponent>(e.entity))
		c->uiRendererDirty = m_Scene->GetComponent<UIRendererComponent>(e.entity)->dirty;
	if (m_Scene->HasComponent<UITextComponent>(e.entity))
		c->fontRendererDirty = m_Scene->GetComponent<UITextComponent>(e.entity)->dirty;

	if (c->parent != 0)
		m_Scene->GetComponent<UIComponent>(c->parent)->childs->push_back(e.entity);
}

void SceneUIHandler::OnComponentAdded(const ComponentAddedEvent<UIHoverComponent>& e)
{
	if (m_Scene->HasComponent<UIRendererComponent>(e.entity))
		m_Scene->GetComponent<UIHoverComponent>(e.entity)->uiRendererDirty = m_Scene->GetComponent<UIRendererComponent>(e.entity)->dirty;
}

void SceneUIHandler::OnComponentAdded(const ComponentAddedEvent<UITextComponent>& e)
{
	if (m_Scene->HasComponent<UIComponent>(e.entity))
		m_Scene->GetComponent<UIComponent>(e.entity)->fontRendererDirty = m_Scene->GetComponent<UITextComponent>(e.entity)->dirty;
}

void SceneUIHandler::OnComponentAdded(const ComponentAddedEvent<UIRendererComponent>& e)
{
	if (m_Scene->HasComponent<UIComponent>(e.entity))
		m_Scene->GetComponent<UIComponent>(e.entity)->uiRendererDirty = m_Scene->GetComponent<UIRendererComponent>(e.entity)->dirty;
}

void SceneUIHandler::OnComponentRemoved(const ComponentRemovedEvent<UIComponent>& e)
{
	auto* c = m_Scene->GetComponent<UIComponent>(e.entity);
	if (c->parent != 0)
	{
		auto* parentComponent = m_Scene->GetComponent<UIComponent>(c->parent);
		parentComponent->childs->erase(std::remove(parentComponent->childs->begin(), parentComponent->childs->end(), e.entity));
	}
}

void SceneUIHandler::OnWindowResized(const WindowResizedEvent& e)
{
	for (auto* c : m_Scene->ViewComponents<UIComponent>())
	{
		bool resized = false;

		if (c->x.mode == UISize::Mode::RelativeToWindow)
		{
			c->x.absoluteValue = c->x.value * e.width;
			resized = true;
		}

		if (c->y.mode == UISize::Mode::RelativeToWindow)
		{
			c->y.absoluteValue = c->y.value * e.height;
			resized = true;
		}

		if (c->width.mode == UISize::Mode::RelativeToWindow)
		{
			c->width.absoluteValue = c->width.value * e.width;
			resized = true;
		}

		if (c->height.mode == UISize::Mode::RelativeToWindow)
		{
			c->height.absoluteValue = c->height.value * e.height;
			resized = true;
		}

		if (resized)
		{
			c->SetRendererDirty();
			ResizeChilds(c);
		}
	}
}

float SceneUIHandler::CalculateAbsoluteX(const UISize& x, UIComponent* parent)
{
	switch (x.mode)
	{
	case UISize::Mode::Absolute: return x.value;
	case UISize::Mode::RelativeToWindow: return x.value * Window::getWidth();
	case UISize::Mode::RelativeToEntity: return parent->GetX() + x.value * parent->GetWidth();
	default:
		SONIC_LOG_ERROR("Unknown UI Size mode");
		return -1.0f;
	}
}

float SceneUIHandler::CalculateAbsoluteY(const UISize& y, UIComponent* parent)
{
	switch (y.mode)
	{
	case UISize::Mode::Absolute: return y.value;
	case UISize::Mode::RelativeToWindow: return y.value * Window::getHeight();
	case UISize::Mode::RelativeToEntity: return parent->GetY() + y.value * parent->GetHeight();
	default:
		SONIC_LOG_ERROR("Unknown UI Size mode");
		return -1.0f;
	}
}

float SceneUIHandler::CalculateAbsoluteWidth(const UISize& width, UIComponent* parent)
{
	switch (width.mode)
	{
	case UISize::Mode::Absolute: return width.value;
	case UISize::Mode::RelativeToWindow: return width.value * Window::getWidth();
	case UISize::Mode::RelativeToEntity: return width.value * parent->GetWidth();
	default:
		SONIC_LOG_ERROR("Unknown UI Size mode");
		return -1.0f;
	}
}

float SceneUIHandler::CalculateAbsoluteHeight(const UISize& height, UIComponent* parent)
{
	switch (height.mode)
	{
	case UISize::Mode::Absolute: return height.value;
	case UISize::Mode::RelativeToWindow: return height.value * Window::getHeight();
	case UISize::Mode::RelativeToEntity: return height.value * parent->GetHeight();
	default:
		SONIC_LOG_ERROR("Unknown UI Size mode");
		return -1.0f;
	}
}

void SceneUIHandler::ResizeChilds(UIComponent* c)
{
	for (EntityID child : *c->childs)
	{
		auto* childComponent = m_Scene->GetComponent<UIComponent>(child);
		if (!childComponent->IsDirty())
		{
			if (childComponent->x.mode == UISize::Mode::RelativeToEntity) childComponent->x.absoluteValue = c->x.absoluteValue + childComponent->x.value * c->width.absoluteValue;
			if (childComponent->y.mode == UISize::Mode::RelativeToEntity) childComponent->y.absoluteValue = c->y.absoluteValue + childComponent->y.value * c->height.absoluteValue;
			if (childComponent->width.mode == UISize::Mode::RelativeToEntity) childComponent->width.absoluteValue = childComponent->width.value * c->width.absoluteValue;
			if (childComponent->height.mode == UISize::Mode::RelativeToEntity) childComponent->height.absoluteValue = childComponent->height.value * c->height.absoluteValue;
			childComponent->SetRendererDirty();
		}
	}
}
