#include <algorithm>
#include "Sonic/Math/Math.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Window/Input/Mouse.h"
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/Views.h"
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
	m_Scene->AddListener(this, &SceneUIHandler::OnMouseButtonPressed);
	m_Scene->AddListener(this, &SceneUIHandler::OnMouseButtonReleased);
	m_Scene->AddListener(this, &SceneUIHandler::OnMouseMoved);
	m_Scene->AddListener(this, &SceneUIHandler::OnWindowResized);

	m_Scene->AddListener<SceneUIHandler, ComponentAddedEvent<UIComponent>>(this, &SceneUIHandler::OnComponentAdded);
	m_Scene->AddListener<SceneUIHandler, ComponentAddedEvent<UIHoverComponent>>(this, &SceneUIHandler::OnComponentAdded);
	m_Scene->AddListener<SceneUIHandler, ComponentAddedEvent<UITextComponent>>(this, &SceneUIHandler::OnComponentAdded);
	m_Scene->AddListener<SceneUIHandler, ComponentAddedEvent<UIRendererComponent>>(this, &SceneUIHandler::OnComponentAdded);
	m_Scene->AddListener<SceneUIHandler, ComponentAddedEvent<UIPositionConstraintsComponent>>(this, &SceneUIHandler::OnComponentAdded);
	m_Scene->AddListener<SceneUIHandler, ComponentAddedEvent<UISizeConstraintsComponent>>(this, &SceneUIHandler::OnComponentAdded);
	m_Scene->AddListener<SceneUIHandler, ComponentRemovedEvent<UIComponent>>(this, &SceneUIHandler::OnComponentRemoved);
}

void SceneUIHandler::Update(float deltaTime)
{
	
}

void SceneUIHandler::OnMouseButtonPressed(const MouseButtonPressedEvent& e)
{
	if (e.button == Buttons::Left)
	{
		for (auto [entity, cl, c] : m_Scene->Group<UIClickListenerComponent, UIComponent>())
		{
			if (Math::isInRange(e.x, c->GetX(), c->GetX() + c->GetWidth()) &&
				Math::isInRange(e.y, c->GetY(), c->GetY() + c->GetHeight()))
			{
				cl->clicked = true;
			}
		}
	}
}

void SceneUIHandler::OnMouseButtonReleased(const MouseButtonReleasedEvent& e)
{
	if (e.button == Buttons::Left)
	{
		if (s_CurrentAction == Action::None)
		{
			m_Scene->Group<UIClickListenerComponent, UIComponent>().ForEach([=](auto entity, auto cl, auto c) {
				if (cl->clicked &&
					Math::isInRange(e.x, c->GetX(), c->GetX() + c->GetWidth()) &&
					Math::isInRange(e.y, c->GetY(), c->GetY() + c->GetHeight()))
				{
					(*cl->listener)(e);
					cl->clicked = false;
				}
			});
			//for (auto [entity, cl, c] : m_Scene->Group<UIClickListenerComponent, UIComponent>())
			//{
			//	if (cl->clicked &&
			//		Math::isInRange(e.x, c->GetX(), c->GetX() + c->GetWidth()) &&
			//		Math::isInRange(e.y, c->GetY(), c->GetY() + c->GetHeight()))
			//	{
			//		(*cl->listener)(e);
			//		cl->clicked = false;
			//	}
			//}
		}

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
				UpdateUIMovableComponentMouseButtonDown(entity, m, c, e);
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
	auto* c = m_Scene->GetComponent<UIComponent>(e.entity);
	auto* parent = c->parent != 0 ? m_Scene->GetComponent<UIComponent>(c->parent) : nullptr;

	switch (c->x.mode)
	{
	case UISize::Mode::Absolute: c->x.absoluteValue = c->x.relativeValue; break;
	case UISize::Mode::RelativeToWindow: c->x.absoluteValue = c->x.relativeValue * Window::getWidth(); break;
	case UISize::Mode::RelativeToEntity: c->x.absoluteValue = parent->x.absoluteValue + c->x.relativeValue * parent->width.absoluteValue; break;
	}

	switch (c->y.mode)
	{
	case UISize::Mode::Absolute: c->y.absoluteValue = c->y.relativeValue; break;
	case UISize::Mode::RelativeToWindow: c->y.absoluteValue = c->y.relativeValue * Window::getHeight(); break;
	case UISize::Mode::RelativeToEntity: c->y.absoluteValue = parent->y.absoluteValue + c->y.relativeValue * parent->height.absoluteValue; break;
	}

	switch (c->width.mode)
	{
	case UISize::Mode::Absolute: c->width.absoluteValue = c->width.relativeValue; break;
	case UISize::Mode::RelativeToWindow: c->width.absoluteValue = c->width.relativeValue * Window::getWidth(); break;
	case UISize::Mode::RelativeToEntity: c->width.absoluteValue = c->width.relativeValue * parent->width.absoluteValue; break;
	}

	switch (c->height.mode)
	{
	case UISize::Mode::Absolute: c->height.absoluteValue = c->height.relativeValue; break;
	case UISize::Mode::RelativeToWindow: c->height.absoluteValue = c->height.relativeValue * Window::getHeight(); break;
	case UISize::Mode::RelativeToEntity: c->height.absoluteValue = c->height.relativeValue * parent->height.absoluteValue; break;
	}

	if (m_Scene->HasComponent<UIRendererComponent>(e.entity))
		c->uiRendererDirty = m_Scene->GetComponent<UIRendererComponent>(e.entity)->dirty;
	if (m_Scene->HasComponent<UITextComponent>(e.entity))
		c->fontRendererDirty = m_Scene->GetComponent<UITextComponent>(e.entity)->dirty;

	if (c->parent != 0)
		m_ChildRegistry[c->parent].push_back(e.entity);

	if (m_Scene->HasComponent<UIPositionConstraintsComponent>(e.entity))
	{
		auto* constraints = m_Scene->GetComponent<UIPositionConstraintsComponent>(e.entity);
		auto* c = m_Scene->GetComponent<UIComponent>(e.entity);

		FitPosition(e.entity, c, constraints);
	}

	if (m_Scene->HasComponent<UISizeConstraintsComponent>(e.entity))
	{
		auto* constraints = m_Scene->GetComponent<UISizeConstraintsComponent>(e.entity);
		auto* c = m_Scene->GetComponent<UIComponent>(e.entity);

		FitSize(e.entity, c, constraints);
	}
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
	if (m_Scene->HasComponent<UIHoverComponent>(e.entity))
		m_Scene->GetComponent<UIHoverComponent>(e.entity)->rendererDirty = m_Scene->GetComponent<UIRendererComponent>(e.entity)->dirty;
}

void SceneUIHandler::OnComponentAdded(const ComponentAddedEvent<UIPositionConstraintsComponent>& e)
{
	if (m_Scene->HasComponent<UIComponent>(e.entity))
	{
		auto* constraints = m_Scene->GetComponent<UIPositionConstraintsComponent>(e.entity);
		auto* c = m_Scene->GetComponent<UIComponent>(e.entity);

		FitPosition(e.entity, c, constraints);
	}
}

void SceneUIHandler::OnComponentAdded(const ComponentAddedEvent<UISizeConstraintsComponent>& e)
{
	if (m_Scene->HasComponent<UIComponent>(e.entity))
	{
		auto* constraints = m_Scene->GetComponent<UISizeConstraintsComponent>(e.entity);
		auto* c = m_Scene->GetComponent<UIComponent>(e.entity);

		FitSize(e.entity, c, constraints);
	}
}

void SceneUIHandler::OnComponentRemoved(const ComponentRemovedEvent<UIComponent>& e)
{
	auto* c = m_Scene->GetComponent<UIComponent>(e.entity);
	if (c->parent != 0)
	{
		std::vector<Entity>& childs = m_ChildRegistry[c->parent];
		childs.erase(std::remove(childs.begin(), childs.end(), e.entity));
	}
}

void SceneUIHandler::OnWindowResized(const WindowResizedEvent& e)
{
	for (auto [entity, c] : m_Scene->View<UIComponent>())
	{
		if (c->x.mode == UISize::Mode::RelativeToWindow)
			SetX(entity, c, c->x.relativeValue * e.width, nullptr, c->width.mode != UISize::Mode::RelativeToWindow);

		if (c->y.mode == UISize::Mode::RelativeToWindow)
			SetY(entity, c, c->y.relativeValue * e.height, nullptr, c->height.mode != UISize::Mode::RelativeToWindow);

		if (c->width.mode == UISize::Mode::RelativeToWindow)
			SetWidth(entity, c, c->width.relativeValue * e.width);

		if (c->height.mode == UISize::Mode::RelativeToWindow)
			SetHeight(entity, c, c->height.relativeValue * e.height);
	}
}

void SceneUIHandler::UpdateUIResizableComponentMouseButtonDown(Entity entity, UIResizableComponent* r, UIComponent* c, const MouseMovedEvent& e)
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

	UIComponent* parent = c->parent != 0 ? m_Scene->GetComponent<UIComponent>(c->parent) : nullptr;

	bool resized = false;
	if (r->bordersHovered.bottom)
	{
		float heightBefore = c->GetHeight();
		SetHeight(entity, c, c->GetHeight() + c->GetY() - e.y, parent);

		float dy = c->GetHeight() - heightBefore;
		SetY(entity, c, c->GetY() - dy, parent);

		resized = true;
	}
	else if (r->bordersHovered.top)
	{
		float dy = e.y - (c->GetY() + c->GetHeight());
		SetHeight(entity, c, c->GetHeight() + dy, parent);

		resized = true;
	}

	if (r->bordersHovered.left)
	{
		float widthBefore = c->GetWidth();
		SetWidth(entity, c, c->GetWidth() + c->GetX() - e.x, parent);

		float dx = c->GetWidth() - widthBefore;
		SetX(entity, c, c->GetX() - dx, parent);

		resized = true;
	}
	else if (r->bordersHovered.right)
	{
		float dx = e.x - (c->GetX() + c->GetWidth());
		SetWidth(entity, c, c->GetWidth() + dx, parent);

		resized = true;
	}

	if (resized)
	{
		s_CurrentAction = Action::Resizing;
		if (r->onResized)
			r->onResized(UIEntityResizedEvent{ c->GetWidth(), c->GetHeight() });
	}
}

void SceneUIHandler::UpdateUIResizableComponentMouseButtonUp(Entity entity, UIResizableComponent* r, UIComponent* c, const MouseMovedEvent& e)
{
	r->bordersHovered = { false, false, false, false };

	if (Math::isInRange(e.beforeX, c->GetX() - r->grabSize, c->GetX() + c->GetWidth() + r->grabSize))
	{
		if (r->bordersResizable.bottom && Math::isInRange(e.beforeY, c->GetY() - r->grabSize, c->GetY() + r->grabSize))
			r->bordersHovered.bottom = true;
		else if (r->bordersResizable.top && Math::isInRange(e.beforeY, c->GetY() + c->GetHeight() - r->grabSize, c->GetY() + c->GetHeight() + r->grabSize))
			r->bordersHovered.top = true;
	}

	if (Math::isInRange(e.beforeY, c->GetY() - r->grabSize, c->GetY() + c->GetHeight() + r->grabSize))
	{
		if (r->bordersResizable.left && Math::isInRange(e.beforeX, c->GetX() - r->grabSize, c->GetX() + r->grabSize))
			r->bordersHovered.left = true;
		else if (r->bordersResizable.right && Math::isInRange(e.beforeX, c->GetX() + c->GetWidth() - r->grabSize, c->GetX() + c->GetWidth() + r->grabSize))
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
		m->hovered = true;
		Window::setCursor(m->cursor);
		s_HoveredAction = Action::Moving;
	}
	else
	{
		m->hovered = false;
	}
}

void SceneUIHandler::UpdateUIMovableComponentMouseButtonDown(Entity entity, UIMovableComponent* m, UIComponent* c, const MouseMovedEvent& e)
{
	if (m->hovered)
	{
		Window::setCursor(m->cursor);
		s_CurrentAction = Action::Moving;

		UIComponent* parent = c->parent != 0 ? m_Scene->GetComponent<UIComponent>(c->parent) : nullptr;
		SetX(entity, c, e.x - c->GetWidth() / 2, parent);
		SetY(entity, c, e.y - c->GetHeight() / 2, parent);

		if (m->onMoved)
			m->onMoved(UIEntityMovedEvent{ e.deltaX, e.deltaY });
	}
}

void SceneUIHandler::SetX(Entity entity, UIComponent* component, float value, UIComponent* parent, bool resizeChilds)
{
	float relativeValue = 0.0f;
	switch (component->x.mode)
	{
	case UISize::Mode::Absolute: relativeValue = value; break;
	case UISize::Mode::RelativeToWindow: relativeValue = value / Window::getWidth(); break;
	case UISize::Mode::RelativeToEntity: relativeValue = (value - parent->x.absoluteValue) / parent->width.absoluteValue; break;
	}

	if (m_Scene->HasComponent<UIPositionConstraintsComponent>(entity))
	{
		auto* constraints = m_Scene->GetComponent<UIPositionConstraintsComponent>(entity);

		if (relativeValue < constraints->minX)
			relativeValue = constraints->minX;
		else if (relativeValue > constraints->maxX)
			relativeValue = constraints->maxX;

		component->x.relativeValue = relativeValue;

		switch (component->x.mode)
		{
		case UISize::Mode::Absolute: component->x.absoluteValue = relativeValue; break;
		case UISize::Mode::RelativeToWindow: component->x.absoluteValue = relativeValue *  Window::getWidth(); break;
		case UISize::Mode::RelativeToEntity: component->x.absoluteValue = parent->x.absoluteValue + relativeValue * parent->width.absoluteValue;
		}
	}
	else
	{
		component->x.absoluteValue = value;
		component->x.relativeValue = relativeValue;
	}

	MarkDirty(component);

	if (resizeChilds)
		ResizeChildsX(entity, component);
}

void SceneUIHandler::SetY(Entity entity, UIComponent* component, float value, UIComponent* parent, bool resizeChilds)
{
	float relativeValue = 0.0f;
	switch (component->y.mode)
	{
	case UISize::Mode::Absolute: relativeValue = value; break;
	case UISize::Mode::RelativeToWindow: relativeValue = value / Window::getHeight(); break;
	case UISize::Mode::RelativeToEntity: relativeValue = (value - parent->y.absoluteValue) / parent->height.absoluteValue; break;
	}

	if (m_Scene->HasComponent<UIPositionConstraintsComponent>(entity))
	{
		auto* constraints = m_Scene->GetComponent<UIPositionConstraintsComponent>(entity);

		if (relativeValue < constraints->minY)
			relativeValue = constraints->minY;
		else if (relativeValue > constraints->maxY)
			relativeValue = constraints->maxY;

		component->y.relativeValue = relativeValue;

		switch (component->y.mode)
		{
		case UISize::Mode::Absolute: component->y.absoluteValue = relativeValue; break;
		case UISize::Mode::RelativeToWindow: component->y.absoluteValue = relativeValue * Window::getHeight(); break;
		case UISize::Mode::RelativeToEntity: component->y.absoluteValue = parent->y.absoluteValue + relativeValue * parent->height.absoluteValue;
		}
	}
	else
	{
		component->y.absoluteValue = value;
		component->y.relativeValue = relativeValue;
	}

	MarkDirty(component);

	if (resizeChilds)
		ResizeChildsY(entity, component);
}

void SceneUIHandler::SetWidth(Entity entity, UIComponent* component, float value, UIComponent* parent)
{
	if (value < 0.0f)
		value = 0.0f;

	float relativeValue = 0.0f;
	switch (component->width.mode)
	{
	case UISize::Mode::Absolute: relativeValue = value; break;
	case UISize::Mode::RelativeToWindow: relativeValue = value / Window::getWidth(); break;
	case UISize::Mode::RelativeToEntity: relativeValue = value / parent->width.absoluteValue; break;
	}

	if (m_Scene->HasComponent<UISizeConstraintsComponent>(entity))
	{
		auto* constraints = m_Scene->GetComponent<UISizeConstraintsComponent>(entity);

		if (relativeValue < constraints->minWidth)
			relativeValue = constraints->minWidth;
		else if (relativeValue > constraints->maxWidth)
			relativeValue = constraints->maxWidth;

		component->width.relativeValue = relativeValue;

		switch (component->width.mode)
		{
		case UISize::Mode::Absolute: component->width.absoluteValue = relativeValue; break;
		case UISize::Mode::RelativeToWindow: component->width.absoluteValue = relativeValue * Window::getWidth(); break;
		case UISize::Mode::RelativeToEntity: component->width.absoluteValue = relativeValue * parent->width.absoluteValue;
		}
	}
	else
	{
		component->width.absoluteValue = value;
		component->width.relativeValue = relativeValue;
	}

	MarkDirty(component);
	ResizeChildsWidth(entity, component);
}

void SceneUIHandler::SetHeight(Entity entity, UIComponent* component, float value, UIComponent* parent)
{
	if (value < 0.0f)
		value = 0.0f;

	float relativeValue = 0.0f;
	switch (component->height.mode)
	{
	case UISize::Mode::Absolute: relativeValue = value; break;
	case UISize::Mode::RelativeToWindow: relativeValue = value / Window::getHeight(); break;
	case UISize::Mode::RelativeToEntity: relativeValue = value / parent->height.absoluteValue; break;
	}

	if (m_Scene->HasComponent<UISizeConstraintsComponent>(entity))
	{
		auto* constraints = m_Scene->GetComponent<UISizeConstraintsComponent>(entity);

		if (relativeValue < constraints->minHeight)
			relativeValue = constraints->minHeight;
		else if (relativeValue > constraints->maxHeight)
			relativeValue = constraints->maxHeight;

		component->height.relativeValue = relativeValue;

		switch (component->height.mode)
		{
		case UISize::Mode::Absolute: component->height.absoluteValue = relativeValue; break;
		case UISize::Mode::RelativeToWindow: component->height.absoluteValue = relativeValue * Window::getHeight(); break;
		case UISize::Mode::RelativeToEntity: component->height.absoluteValue = relativeValue * parent->height.absoluteValue;
		}
	}
	else
	{
		component->height.absoluteValue = value;
		component->height.relativeValue = relativeValue;
	}

	MarkDirty(component);
	ResizeChildsHeight(entity, component);
}

void SceneUIHandler::MarkDirty(UIComponent* component)
{
	if (component->uiRendererDirty)
		*component->uiRendererDirty = true;
	if (component->fontRendererDirty)
		*component->fontRendererDirty = true;
}

void SceneUIHandler::ResizeChildsX(Entity parent, UIComponent* c)
{
	for (Entity child : m_ChildRegistry[parent])
	{
		UIComponent* childComponent = m_Scene->GetComponent<UIComponent>(child);
		if (childComponent->x.mode == UISize::Mode::RelativeToEntity)
		{
			childComponent->x.absoluteValue = c->x.absoluteValue + childComponent->x.relativeValue * c->width.absoluteValue;
			ResizeChildsX(child, childComponent);
			MarkDirty(childComponent);
		}
	}
}

void SceneUIHandler::ResizeChildsY(Entity parent, UIComponent* c)
{
	for (Entity child : m_ChildRegistry[parent])
	{
		UIComponent* childComponent = m_Scene->GetComponent<UIComponent>(child);
		childComponent->y.absoluteValue = c->y.absoluteValue + childComponent->y.relativeValue * c->height.absoluteValue;
		ResizeChildsY(child, childComponent);
		MarkDirty(childComponent);
	}
}

void SceneUIHandler::ResizeChildsWidth(Entity parent, UIComponent* c)
{
	for (Entity child : m_ChildRegistry[parent])
	{
		UIComponent* childComponent = m_Scene->GetComponent<UIComponent>(child);

		if (childComponent->x.mode == UISize::Mode::RelativeToEntity)
		{
			childComponent->x.absoluteValue = c->x.absoluteValue + childComponent->x.relativeValue * c->width.absoluteValue;
			if (childComponent->width.mode != UISize::Mode::RelativeToEntity)
			{
				ResizeChildsX(child, childComponent);
				MarkDirty(childComponent);
			}
		}

		if (childComponent->width.mode == UISize::Mode::RelativeToEntity)
		{
			childComponent->width.absoluteValue = childComponent->width.relativeValue * c->width.absoluteValue;
			ResizeChildsWidth(child, childComponent);
			MarkDirty(childComponent);
		}
	}
}

void SceneUIHandler::ResizeChildsHeight(Entity parent, UIComponent* c)
{
	for (Entity child : m_ChildRegistry[parent])
	{
		UIComponent* childComponent = m_Scene->GetComponent<UIComponent>(child);

		if (childComponent->y.mode == UISize::Mode::RelativeToEntity)
		{
			childComponent->y.absoluteValue = c->y.absoluteValue + childComponent->y.relativeValue * c->height.absoluteValue;
			if (childComponent->height.mode != UISize::Mode::RelativeToEntity)
			{
				ResizeChildsY(child, childComponent);
				MarkDirty(childComponent);
			}
		}

		if (childComponent->height.mode == UISize::Mode::RelativeToEntity)
		{
			childComponent->height.absoluteValue = childComponent->height.relativeValue * c->height.absoluteValue;
			ResizeChildsHeight(child, childComponent);
			MarkDirty(childComponent);
		}
	}
}

void SceneUIHandler::FitPosition(Entity entity, UIComponent* c, UIPositionConstraintsComponent* constraints)
{
	auto* parent = c->parent != 0 ? m_Scene->GetComponent<UIComponent>(c->parent) : nullptr;

	if (c->x.absoluteValue < constraints->minX)
	{
		c->x.relativeValue = constraints->minX;
		switch (c->x.mode)
		{
		case UISize::Mode::Absolute: c->x.absoluteValue = constraints->minX; break;
		case UISize::Mode::RelativeToWindow: c->x.absoluteValue = constraints->minX * Window::getWidth(); break;
		case UISize::Mode::RelativeToEntity: c->x.absoluteValue = parent->x.absoluteValue + constraints->minX * parent->width.absoluteValue; break;
		}

		ResizeChildsX(entity, c);
		MarkDirty(c);
	}
	else if (c->x.absoluteValue > constraints->maxX)
	{
		c->x.relativeValue = constraints->maxX;
		switch (c->x.mode)
		{
		case UISize::Mode::Absolute: c->x.absoluteValue = constraints->maxX; break;
		case UISize::Mode::RelativeToWindow: c->x.absoluteValue = constraints->maxX * Window::getWidth(); break;
		case UISize::Mode::RelativeToEntity: c->x.absoluteValue = parent->x.absoluteValue + constraints->maxX * parent->width.absoluteValue; break;
		}

		ResizeChildsX(entity, c);
		MarkDirty(c);
	}

	if (c->y.absoluteValue < constraints->minY)
	{
		c->y.relativeValue = constraints->minY;
		switch (c->y.mode)
		{
		case UISize::Mode::Absolute: c->y.absoluteValue = constraints->minY; break;
		case UISize::Mode::RelativeToWindow: c->y.absoluteValue = constraints->minY * Window::getHeight(); break;
		case UISize::Mode::RelativeToEntity: c->y.absoluteValue = parent->y.absoluteValue + constraints->minY * parent->height.absoluteValue; break;
		}

		ResizeChildsY(entity, c);
		MarkDirty(c);
	}
	else if (c->y.absoluteValue > constraints->maxY)
	{
		c->y.relativeValue = constraints->maxY;
		switch (c->y.mode)
		{
		case UISize::Mode::Absolute: c->y.absoluteValue = constraints->maxY; break;
		case UISize::Mode::RelativeToWindow: c->y.absoluteValue = constraints->maxY * Window::getHeight(); break;
		case UISize::Mode::RelativeToEntity: c->y.absoluteValue = parent->y.absoluteValue + constraints->maxY * parent->height.absoluteValue; break;
		}

		ResizeChildsY(entity, c);
		MarkDirty(c);
	}
}

void SceneUIHandler::FitSize(Entity entity, UIComponent* c, UISizeConstraintsComponent* constraints)
{
	auto* parent = c->parent != 0 ? m_Scene->GetComponent<UIComponent>(c->parent) : nullptr;

	if (c->width.absoluteValue < constraints->minWidth)
	{
		c->width.relativeValue = constraints->minWidth;
		switch (c->width.mode)
		{
		case UISize::Mode::Absolute: c->width.absoluteValue = constraints->minWidth; break;
		case UISize::Mode::RelativeToWindow: c->width.absoluteValue = constraints->minWidth * Window::getWidth(); break;
		case UISize::Mode::RelativeToEntity: c->width.absoluteValue = constraints->minWidth * parent->width.absoluteValue; break;
		}

		ResizeChildsWidth(entity, c);
		MarkDirty(c);
	}
	else if (c->width.absoluteValue > constraints->maxWidth)
	{
		c->width.relativeValue = constraints->maxWidth;
		switch (c->width.mode)
		{
		case UISize::Mode::Absolute: c->width.absoluteValue = constraints->maxWidth; break;
		case UISize::Mode::RelativeToWindow: c->width.absoluteValue = constraints->maxWidth * Window::getWidth(); break;
		case UISize::Mode::RelativeToEntity: c->width.absoluteValue = constraints->maxWidth * parent->width.absoluteValue; break;
		}

		ResizeChildsWidth(entity, c);
		MarkDirty(c);
	}

	if (c->height.absoluteValue < constraints->minHeight)
	{
		c->height.relativeValue = constraints->minHeight;
		switch (c->height.mode)
		{
		case UISize::Mode::Absolute: c->height.absoluteValue = constraints->minHeight; break;
		case UISize::Mode::RelativeToWindow: c->height.absoluteValue = constraints->minHeight * Window::getHeight(); break;
		case UISize::Mode::RelativeToEntity: c->height.absoluteValue = constraints->minHeight * parent->height.absoluteValue; break;
		}

		ResizeChildsHeight(entity, c);
		MarkDirty(c);
	}
	else if (c->height.absoluteValue > constraints->maxHeight)
	{
		c->height.relativeValue = constraints->maxHeight;
		switch (c->height.mode)
		{
		case UISize::Mode::Absolute: c->height.absoluteValue = constraints->maxHeight; break;
		case UISize::Mode::RelativeToWindow: c->height.absoluteValue = constraints->maxHeight * Window::getHeight(); break;
		case UISize::Mode::RelativeToEntity: c->height.absoluteValue = constraints->maxHeight * parent->height.absoluteValue; break;
		}

		ResizeChildsHeight(entity, c);
		MarkDirty(c);
	}
}
