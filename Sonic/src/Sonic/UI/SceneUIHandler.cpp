#include <algorithm>
#include "Sonic/Log/Log.h"
#include "Sonic/Math/Math.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Window/Input/Mouse.h"
#include "Sonic/Event/Events.h"
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/Views.h"
#include "UIComponents.h"
#include "SceneUIHandler.h"

using namespace Sonic;


enum class Action : uint8_t
{
	Resizing,
	Moving,
	None
};

static Action s_CurrentAction = Action::None;
static Action s_HoveredAction = Action::None;


SceneUIHandler::SceneUIHandler(Scene* scene)
	: m_Scene(scene)
{
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

void SceneUIHandler::OnMouseButtonReleased(const MouseButtonReleasedEvent& e)
{
	if (e.button == Buttons::Left)
	{
		if (s_CurrentAction == Action::None)
			for (auto [entity, clickListener, c] : m_Scene->Group<UIClickListenerComponent, UIComponent>())
				if (e.x >= c->GetX() && e.x < c->GetX() + c->GetWidth() && e.y >= c->GetY() && e.y < c->GetY() + c->GetHeight())
					clickListener->listener(e);
		s_CurrentAction = Action::None;
	}
}

void SceneUIHandler::OnMouseMoved(const MouseMovedEvent& e)
{
	Window::setCursor(StandardCursors::Arrow);

	if (Mouse::isButtonPressed(Buttons::Left))
	{
		if (s_CurrentAction == Action::None || s_CurrentAction == Action::Resizing)
		{
			for (auto [entity, r, c] : m_Scene->Group<UIResizableComponent, UIComponent>())
				UpdateUIResizableComponentMouseButtonDown(entity, r, c, e);
		}

		if (s_CurrentAction == Action::None || s_CurrentAction == Action::Moving)
		{
			for (auto [entity, m, c] : m_Scene->Group<UIMovableComponent, UIComponent>())
				UpdateUIMovableComponentMouseButtonDown(m, c, e);
		}
	}
	else
	{
		s_HoveredAction = Action::None;

		for (auto [entity, r, c] : m_Scene->Group<UIResizableComponent, UIComponent>())
			UpdateUIResizableComponentMouseButtonUp(entity, r, c, e);

		if (s_HoveredAction != Action::Resizing)
		{
			for (auto [entity, m, c] : m_Scene->Group<UIMovableComponent, UIComponent>())
				UpdateUIMovableComponentMouseButtonUp(m, c, e);
		}

		for (auto [entity, h, c] : m_Scene->Group<UIHoverComponent, UIComponent>())
			h->SetHovered(e.x >= c->GetX() && e.x < c->GetX() + c->GetWidth() && e.y >= c->GetY() && e.y < c->GetY() + c->GetHeight());
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
		m_Scene->GetComponent<UIHoverComponent>(e.entity)->rendererDirty = m_Scene->GetComponent<UIRendererComponent>(e.entity)->dirty;
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
			c->x.absoluteValue = c->x.relativeValue * e.width;
			resized = true;
		}

		if (c->y.mode == UISize::Mode::RelativeToWindow)
		{
			c->y.absoluteValue = c->y.relativeValue * e.height;
			resized = true;
		}

		if (c->width.mode == UISize::Mode::RelativeToWindow)
		{
			c->width.absoluteValue = c->width.relativeValue * e.width;
			resized = true;
		}

		if (c->height.mode == UISize::Mode::RelativeToWindow)
		{
			c->height.absoluteValue = c->height.relativeValue * e.height;
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
	case UISize::Mode::Absolute: return x.relativeValue;
	case UISize::Mode::RelativeToWindow: return x.relativeValue * Window::getWidth();
	case UISize::Mode::RelativeToEntity: return parent->GetX() + x.relativeValue * parent->GetWidth();
	default:
		SONIC_LOG_ERROR("Unknown UI Size mode");
		return -1.0f;
	}
}

float SceneUIHandler::CalculateAbsoluteY(const UISize& y, UIComponent* parent)
{
	switch (y.mode)
	{
	case UISize::Mode::Absolute: return y.relativeValue;
	case UISize::Mode::RelativeToWindow: return y.relativeValue * Window::getHeight();
	case UISize::Mode::RelativeToEntity: return parent->GetY() + y.relativeValue * parent->GetHeight();
	default:
		SONIC_LOG_ERROR("Unknown UI Size mode");
		return -1.0f;
	}
}

float SceneUIHandler::CalculateAbsoluteWidth(const UISize& width, UIComponent* parent)
{
	switch (width.mode)
	{
	case UISize::Mode::Absolute: return width.relativeValue;
	case UISize::Mode::RelativeToWindow: return width.relativeValue * Window::getWidth();
	case UISize::Mode::RelativeToEntity: return width.relativeValue * parent->GetWidth();
	default:
		SONIC_LOG_ERROR("Unknown UI Size mode");
		return -1.0f;
	}
}

float SceneUIHandler::CalculateAbsoluteHeight(const UISize& height, UIComponent* parent)
{
	switch (height.mode)
	{
	case UISize::Mode::Absolute: return height.relativeValue;
	case UISize::Mode::RelativeToWindow: return height.relativeValue * Window::getHeight();
	case UISize::Mode::RelativeToEntity: return height.relativeValue * parent->GetHeight();
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
			if (childComponent->x.mode == UISize::Mode::RelativeToEntity) childComponent->x.absoluteValue = c->x.absoluteValue + childComponent->x.relativeValue * c->width.absoluteValue;
			if (childComponent->y.mode == UISize::Mode::RelativeToEntity) childComponent->y.absoluteValue = c->y.absoluteValue + childComponent->y.relativeValue * c->height.absoluteValue;
			if (childComponent->width.mode == UISize::Mode::RelativeToEntity) childComponent->width.absoluteValue = childComponent->width.relativeValue * c->width.absoluteValue;
			if (childComponent->height.mode == UISize::Mode::RelativeToEntity) childComponent->height.absoluteValue = childComponent->height.relativeValue * c->height.absoluteValue;
			childComponent->SetRendererDirty();
		}
	}
}

void SceneUIHandler::UpdateUIResizableComponentMouseButtonDown(EntityID entity, UIResizableComponent* r, UIComponent* c, const MouseMovedEvent& e)
{
	if ((r->bordersHovered.bottom && r->bordersHovered.right) || (r->bordersHovered.top && r->bordersHovered.left))
		Window::setCursor(StandardCursors::ResizeDiagonalLeft);
	else if ((r->bordersHovered.bottom && r->bordersHovered.left) || (r->bordersHovered.top && r->bordersHovered.right))
		Window::setCursor(StandardCursors::ResizeDiagonalRight);
	else if (r->bordersHovered.bottom || r->bordersHovered.top)
		Window::setCursor(StandardCursors::ResizeVertical);
	else if (r->bordersHovered.left || r->bordersHovered.right)
		Window::setCursor(StandardCursors::ResizeHorizontal);

	if (r->bordersHovered)
	{
		if (m_Scene->HasComponent<UIHoverComponent>(entity))
			m_Scene->GetComponent<UIHoverComponent>(entity)->SetHovered(true);
		s_HoveredAction = Action::Resizing;
	}

	if (r->bordersHovered.bottom)
	{
		float dy = std::clamp<float>(c->GetY() - e.y, r->minHeight.absoluteValue - c->GetHeight(), r->maxHeight.absoluteValue - c->GetHeight());
		c->SetY(c->GetY() - dy);
		c->SetHeight(c->GetHeight() + dy);
		s_CurrentAction = Action::Resizing;
	}
	else if (r->bordersHovered.top)
	{
		float dy = std::clamp<float>(e.y - (c->GetY() + c->GetHeight()), r->minHeight.absoluteValue - c->GetHeight(), r->maxHeight.absoluteValue - c->GetHeight());
		c->SetHeight(c->GetHeight() + dy);
		s_CurrentAction = Action::Resizing;
	}

	if (r->bordersHovered.left)
	{
		float dx = std::clamp<float>(c->GetX() - e.x, r->minWidth.absoluteValue - c->GetWidth(), r->maxWidth.absoluteValue - c->GetWidth());
		c->SetX(c->GetX() - dx);
		c->SetWidth(c->GetWidth() + dx);
		s_CurrentAction = Action::Resizing;
	}
	else if (r->bordersHovered.right)
	{
		float dx = std::clamp<float>(e.x - (c->GetX() + c->GetWidth()), r->minWidth.absoluteValue - c->GetWidth(), r->maxWidth.absoluteValue - c->GetWidth());
		c->SetWidth(c->GetWidth() + dx);
		s_CurrentAction = Action::Resizing;
	}
}

void SceneUIHandler::UpdateUIResizableComponentMouseButtonUp(EntityID entity, UIResizableComponent* r, UIComponent* c, const MouseMovedEvent& e)
{
	r->bordersHovered = { false, false, false, false };

	if (e.beforeX >= c->GetX() - r->grabSize && e.beforeX < c->GetX() + c->GetWidth() + r->grabSize)
	{
		if (r->bordersResizable.bottom && e.beforeY >= c->GetY() - r->grabSize && e.beforeY < c->GetY() + r->grabSize)
			r->bordersHovered.bottom = true;
		else if (r->bordersResizable.top && e.beforeY >= c->GetY() + c->GetHeight() - r->grabSize && e.beforeY < c->GetY() + c->GetHeight() + r->grabSize)
			r->bordersHovered.top = true;
	}

	if (e.beforeY >= c->GetY() - r->grabSize && e.beforeY < c->GetY() + c->GetHeight() + r->grabSize)
	{
		if (r->bordersResizable.left && e.beforeX >= c->GetX() - r->grabSize && e.beforeX < c->GetX() + r->grabSize)
			r->bordersHovered.left = true;
		else if (r->bordersResizable.right && e.beforeX >= c->GetX() + c->GetWidth() - r->grabSize && e.beforeX < c->GetX() + c->GetWidth() + r->grabSize)
			r->bordersHovered.right = true;
	}

	if ((r->bordersHovered.bottom && r->bordersHovered.right) || (r->bordersHovered.top && r->bordersHovered.left))
		Window::setCursor(StandardCursors::ResizeDiagonalLeft);
	else if ((r->bordersHovered.bottom && r->bordersHovered.left) || (r->bordersHovered.top && r->bordersHovered.right))
		Window::setCursor(StandardCursors::ResizeDiagonalRight);
	else if (r->bordersHovered.bottom || r->bordersHovered.top)
		Window::setCursor(StandardCursors::ResizeVertical);
	else if (r->bordersHovered.left || r->bordersHovered.right)
		Window::setCursor(StandardCursors::ResizeHorizontal);

	if (r->bordersHovered)
	{
		if (m_Scene->HasComponent<UIHoverComponent>(entity))
			m_Scene->GetComponent<UIHoverComponent>(entity)->SetHovered(true);
		s_HoveredAction = Action::Resizing;
	}
}

void SceneUIHandler::UpdateUIMovableComponentMouseButtonUp(UIMovableComponent* m, UIComponent* c, const MouseMovedEvent& e)
{
	if (Sonic::Math::isInRange(e.beforeX, c->GetX(), c->GetX() + c->GetWidth()) &&
		Sonic::Math::isInRange(e.beforeY, c->GetY(), c->GetY() + c->GetHeight()))
	{
		Window::setCursor(StandardCursors::Move);
		s_HoveredAction = Action::Moving;
	}
}

void SceneUIHandler::UpdateUIMovableComponentMouseButtonDown(UIMovableComponent* m, UIComponent* c, const MouseMovedEvent& e)
{
	if (Sonic::Math::isInRange(e.beforeX, c->GetX(), c->GetX() + c->GetWidth()) &&
		Sonic::Math::isInRange(e.beforeY, c->GetY(), c->GetY() + c->GetHeight()))
	{
		Window::setCursor(StandardCursors::Move);
		s_CurrentAction = Action::Moving;

		c->SetX(c->GetX() + e.deltaX);
		c->SetY(c->GetY() + e.deltaY);
	}
}
