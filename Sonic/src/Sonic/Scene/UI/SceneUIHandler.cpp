#include <algorithm>
#include "Sonic/Util/Math/Math.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Window/Input/Mouse.h"
#include "Sonic/Scene/Scene.h"
#include "Sonic/Scene/ECS/Views.h"
#include "Sonic/Renderer/UI/UIRenderer.h"
#include "Sonic/Renderer/Font/FontRenderer.h"
#include "UIUtils.h"
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
}

void SceneUIHandler::Init()
{
	EventDispatcher::addListener(this, &SceneUIHandler::OnMouseButtonPressed);
	EventDispatcher::addListener(this, &SceneUIHandler::OnMouseButtonReleased);
	EventDispatcher::addListener(this, &SceneUIHandler::OnMouseMoved);
	EventDispatcher::addListener(this, &SceneUIHandler::OnWindowResized);

	EventDispatcher::addListener<SceneUIHandler, ComponentAddedEvent<UIComponent>>(this, &SceneUIHandler::OnComponentAdded);
	EventDispatcher::addListener<SceneUIHandler, ComponentAddedEvent<UIHoverComponent>>(this, &SceneUIHandler::OnComponentAdded);
	EventDispatcher::addListener<SceneUIHandler, ComponentAddedEvent<UITextComponent>>(this, &SceneUIHandler::OnComponentAdded);
	EventDispatcher::addListener<SceneUIHandler, ComponentAddedEvent<UIRendererComponent>>(this, &SceneUIHandler::OnComponentAdded);
	EventDispatcher::addListener<SceneUIHandler, ComponentAddedEvent<UIPositionConstraintsComponent>>(this, &SceneUIHandler::OnComponentAdded);
	EventDispatcher::addListener<SceneUIHandler, ComponentAddedEvent<UISizeConstraintsComponent>>(this, &SceneUIHandler::OnComponentAdded);
	EventDispatcher::addListener<SceneUIHandler, ComponentRemovedEvent<UIComponent>>(this, &SceneUIHandler::OnComponentRemoved);
	EventDispatcher::addListener<SceneUIHandler, ComponentRemovedEvent<UIRendererComponent>>(this, &SceneUIHandler::OnComponentRemoved);
	EventDispatcher::addListener<SceneUIHandler, ComponentRemovedEvent<UITextComponent>>(this, &SceneUIHandler::OnComponentRemoved);
	EventDispatcher::addListener<SceneUIHandler, ComponentDeactivatedEvent<UIComponent>>(this, &SceneUIHandler::OnComponentDeactivated);
	EventDispatcher::addListener<SceneUIHandler, ComponentDeactivatedEvent<UITextComponent>>(this, &SceneUIHandler::OnComponentDeactivated);
	EventDispatcher::addListener<SceneUIHandler, ComponentDeactivatedEvent<UIRendererComponent>>(this, &SceneUIHandler::OnComponentDeactivated);
	EventDispatcher::addListener<SceneUIHandler, ComponentReactivatedEvent<UIComponent>>(this, &SceneUIHandler::OnComponentReactivated);
	EventDispatcher::addListener<SceneUIHandler, ComponentReactivatedEvent<UITextComponent>>(this, &SceneUIHandler::OnComponentReactivated);
	EventDispatcher::addListener<SceneUIHandler, ComponentReactivatedEvent<UIRendererComponent>>(this, &SceneUIHandler::OnComponentReactivated);
	EventDispatcher::addListener<SceneUIHandler, ComponentReactivatedEvent<UIPositionConstraintsComponent>>(this, &SceneUIHandler::OnComponentReactivated);
	EventDispatcher::addListener<SceneUIHandler, ComponentReactivatedEvent<UISizeConstraintsComponent>>(this, &SceneUIHandler::OnComponentReactivated);
	EventDispatcher::addListener(this, &SceneUIHandler::OnEntityDeactivated);
	EventDispatcher::addListener(this, &SceneUIHandler::OnEntityReactivated);

	for (auto [e, h, r] : m_Scene->Group<UIHoverComponent, UIRendererComponent>())
		h->rendererDirty = r->dirty;
	for (auto [e, c, r] : m_Scene->Group<UIComponent, UIRendererComponent>())
		c->uiRendererDirty = r->dirty;
	for (auto [e, c, t] : m_Scene->Group<UIComponent, UITextComponent>())
		c->fontRendererDirty = t->dirty;

	for (auto [e, c] : m_Scene->View<UIComponent>())
	{
		UI::recalculate(m_Scene, c);

		if (c->parent != 0)
			m_Scene->GetComponent<UIComponent>(c->parent)->childs->push_back(e);
	}
}

void SceneUIHandler::OnMouseButtonPressed(const MouseButtonPressedEvent& e)
{
	if (e.button == MouseButtons::Left)
	{
		for (auto [entity, cl, c] : m_Scene->Group<UIClickListenerComponent, UIComponent>())
		{
			if (Math::isInRange(e.x, c->x, c->x + c->width) &&
				Math::isInRange(e.y, c->y, c->y + c->height))
			{
				cl->clicked = true;
			}
		}
	}
}

void SceneUIHandler::OnMouseButtonReleased(const MouseButtonReleasedEvent& e)
{
	if (e.button == MouseButtons::Left)
	{
		if (s_CurrentAction == Action::None)
		{
			m_Scene->Group<UIClickListenerComponent, UIComponent>().ForEach([=](auto entity, auto cl, auto c) {
				if (cl->clicked &&
					Math::isInRange(e.x, c->x, c->x + c->width) &&
					Math::isInRange(e.y, c->y, c->y + c->height))
				{
					(*cl->listener)(UIEntityClickedEvent{ e.x - c->x, e.y - c->y, e.x, e.y, e.button });
					cl->clicked = false;
				}
			});
		}

		s_CurrentAction = Action::None;
	}
}

void SceneUIHandler::OnMouseMoved(const MouseMovedEvent& e)
{
	Window::setCursor(Cursors::Arrow);

	if (Mouse::isButtonPressed(MouseButtons::Left))
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
			h->SetHovered(e.x >= c->x && e.x < c->x + c->width && e.y >= c->y && e.y < c->y + c->height);
	}
}

void SceneUIHandler::OnComponentAdded(const ComponentAddedEvent<UIComponent>& e)
{
	auto* c = m_Scene->GetComponent<UIComponent>(e.entity);
	UI::recalculate(m_Scene, c);

	if (c->parent != 0)
		m_Scene->GetComponent<UIComponent>(c->parent)->childs->push_back(e.entity);
	if (m_Scene->HasComponent<UIRendererComponent>(e.entity))
		c->uiRendererDirty = m_Scene->GetComponent<UIRendererComponent>(e.entity)->dirty;
	if (m_Scene->HasComponent<UITextComponent>(e.entity))
		c->fontRendererDirty = m_Scene->GetComponent<UITextComponent>(e.entity)->dirty;
}

void SceneUIHandler::OnComponentAdded(const ComponentAddedEvent<UIHoverComponent>& e)
{
	if (m_Scene->HasComponent<UIRendererComponent>(e.entity))
		m_Scene->GetComponent<UIHoverComponent>(e.entity)->rendererDirty = m_Scene->GetComponent<UIRendererComponent>(e.entity)->dirty;
}

void SceneUIHandler::OnComponentAdded(const ComponentAddedEvent<UITextComponent>& e)
{
	FontRenderer::markDirty();

	if (m_Scene->HasComponent<UIComponent>(e.entity))
		m_Scene->GetComponent<UIComponent>(e.entity)->fontRendererDirty = m_Scene->GetComponent<UITextComponent>(e.entity)->dirty;
}

void SceneUIHandler::OnComponentAdded(const ComponentAddedEvent<UIRendererComponent>& e)
{
	UIRenderer::markDirty();

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

		UI::checkPosition(m_Scene, c, constraints);
	}
}

void SceneUIHandler::OnComponentAdded(const ComponentAddedEvent<UISizeConstraintsComponent>& e)
{
	if (m_Scene->HasComponent<UIComponent>(e.entity))
	{
		auto* constraints = m_Scene->GetComponent<UISizeConstraintsComponent>(e.entity);
		auto* c = m_Scene->GetComponent<UIComponent>(e.entity);

		UI::checkSize(m_Scene, c, constraints);
	}
}

void SceneUIHandler::OnComponentRemoved(const ComponentRemovedEvent<UIComponent>& e)
{
	auto* c = m_Scene->GetComponent<UIComponent>(e.entity);
	if (c->parent != 0)
	{
		std::vector<Entity>& childs = *m_Scene->GetComponent<UIComponent>(c->parent)->childs;
		childs.erase(std::remove(childs.begin(), childs.end(), e.entity));

		if (c->childs->size() != 0)
			SONIC_LOG_WARN("Deactivating UI Entity that has childs. Make sure to deactivate the childs first");
	}
}

void SceneUIHandler::OnComponentRemoved(const ComponentRemovedEvent<UIRendererComponent>& e)
{
	UIRenderer::markDirty();
}

void SceneUIHandler::OnComponentRemoved(const ComponentRemovedEvent<UITextComponent>& e)
{
	FontRenderer::markDirty();
}

void SceneUIHandler::OnEntityDeactivated(const EntityDeactivatedEvent& e)
{
	if (m_Scene->HasComponent<UIRendererComponent>(e.entity))
	{
		UIRenderer::markDirty();
		FontRenderer::markDirty();
	}

	if (m_Scene->HasComponent<UIComponent>(e.entity))
	{
		auto* c = m_Scene->GetComponent<UIComponent>(e.entity);
		if (c->parent != 0)
		{
			std::vector<Entity>& childs = *m_Scene->GetComponent<UIComponent>(c->parent)->childs;
			childs.erase(std::remove(childs.begin(), childs.end(), e.entity));

			if (c->childs->size() != 0)
				SONIC_LOG_WARN("Deactivating UI Entity that has childs. Make sure to deactivate the childs first");
		}
	}
}

void SceneUIHandler::OnEntityReactivated(const EntityReactivatedEvent& e)
{
	if (m_Scene->HasComponent<UIRendererComponent>(e.entity))
		UIRenderer::markDirty();
	if (m_Scene->HasComponent<UITextComponent>(e.entity))
		FontRenderer::markDirty();

	if (m_Scene->HasComponent<UIComponent>(e.entity))
	{
		auto* c = m_Scene->GetComponent<UIComponent>(e.entity);
		UI::recalculate(m_Scene, c);

		if (c->parent != 0)
			m_Scene->GetComponent<UIComponent>(c->parent)->childs->push_back(e.entity);
		if (m_Scene->HasComponent<UIRendererComponent>(e.entity))
			c->uiRendererDirty = m_Scene->GetComponent<UIRendererComponent>(e.entity)->dirty;
		if (m_Scene->HasComponent<UITextComponent>(e.entity))
			c->fontRendererDirty = m_Scene->GetComponent<UITextComponent>(e.entity)->dirty;
	}
}

void SceneUIHandler::OnComponentDeactivated(const ComponentDeactivatedEvent<UIComponent>& e)
{
	auto* c = m_Scene->GetComponent<UIComponent>(e.entity);
	if (c->parent != 0)
	{
		std::vector<Entity>& childs = *m_Scene->GetComponent<UIComponent>(c->parent)->childs;
		childs.erase(std::remove(childs.begin(), childs.end(), e.entity));

		if (c->childs->size() != 0)
			SONIC_LOG_WARN("Deactivating UI Entity that has childs. Make sure to deactivate the childs first");
	}
}

void SceneUIHandler::OnComponentDeactivated(const ComponentDeactivatedEvent<UITextComponent>& e)
{
	FontRenderer::markDirty();
}

void SceneUIHandler::OnComponentDeactivated(const ComponentDeactivatedEvent<UIRendererComponent>& e)
{
	UIRenderer::markDirty();
}

void SceneUIHandler::OnComponentReactivated(const ComponentReactivatedEvent<UIComponent>& e)
{
	auto* c = m_Scene->GetComponent<UIComponent>(e.entity);
	UI::recalculate(m_Scene, c);

	if (c->parent != 0)
		m_Scene->GetComponent<UIComponent>(c->parent)->childs->push_back(e.entity);
}

void SceneUIHandler::OnComponentReactivated(const ComponentReactivatedEvent<UITextComponent>& e)
{
	FontRenderer::markDirty();
}

void SceneUIHandler::OnComponentReactivated(const ComponentReactivatedEvent<UIRendererComponent>& e)
{
	UIRenderer::markDirty();
}

void SceneUIHandler::OnComponentReactivated(const ComponentReactivatedEvent<UIPositionConstraintsComponent>& e)
{
	if (m_Scene->HasComponent<UIComponent>(e.entity))
	{
		auto* constraints = m_Scene->GetComponent<UISizeConstraintsComponent>(e.entity);
		auto* c = m_Scene->GetComponent<UIComponent>(e.entity);

		UI::checkSize(m_Scene, c, constraints);
	}
}

void SceneUIHandler::OnComponentReactivated(const ComponentReactivatedEvent<UISizeConstraintsComponent>& e)
{
	if (m_Scene->HasComponent<UIComponent>(e.entity))
	{
		auto* constraints = m_Scene->GetComponent<UISizeConstraintsComponent>(e.entity);
		auto* c = m_Scene->GetComponent<UIComponent>(e.entity);

		UI::checkSize(m_Scene, c, constraints);
	}
}

void SceneUIHandler::OnWindowResized(const WindowResizedEvent& e)
{
	for (auto c : m_Scene->ViewComponents<UIComponent>())
		UI::recalculate(m_Scene, c);
}

void SceneUIHandler::UpdateUIResizableComponentMouseButtonDown(Entity entity, UIResizableComponent* r, UIComponent* c, const MouseMovedEvent& e)
{
	if ((r->bordersHovered.bottom && r->bordersHovered.right) || (r->bordersHovered.top && r->bordersHovered.left))
		Window::setCursor(Cursors::ResizeDiagonalLeft);
	else if ((r->bordersHovered.bottom && r->bordersHovered.left) || (r->bordersHovered.top && r->bordersHovered.right))
		Window::setCursor(Cursors::ResizeDiagonalRight);
	else if (r->bordersHovered.bottom || r->bordersHovered.top)
		Window::setCursor(Cursors::ResizeVertical);
	else if (r->bordersHovered.left || r->bordersHovered.right)
		Window::setCursor(Cursors::ResizeHorizontal);

	if (r->bordersHovered)
	{
		if (m_Scene->HasComponent<UIHoverComponent>(entity))
			m_Scene->GetComponent<UIHoverComponent>(entity)->SetHovered(true);
		s_HoveredAction = Action::Resizing;
	}

	auto* positionConstraints = m_Scene->HasComponent<UIPositionConstraintsComponent>(entity) ?
		m_Scene->GetComponent<UIPositionConstraintsComponent>(entity) : nullptr;
	auto* sizeConstraints = m_Scene->HasComponent<UISizeConstraintsComponent>(entity) ?
		m_Scene->GetComponent<UISizeConstraintsComponent>(entity) : nullptr;

	bool resized = false;
	if (r->bordersHovered.bottom)
	{
		float heightBefore = c->height;
		UI::setHeight(m_Scene, c, c->height + c->y - e.y, sizeConstraints);
		float dy = c->height - heightBefore;
		UI::setY(m_Scene, c, c->y - dy, positionConstraints);
		resized = true;
	}
	else if (r->bordersHovered.top)
	{
		float dy = e.y - (c->y + c->height);
		UI::setHeight(m_Scene, c, c->height + dy, sizeConstraints);
		resized = true;
	}

	if (r->bordersHovered.left)
	{
		float widthBefore = c->width;
		UI::setWidth(m_Scene, c, c->width + c->x - e.x, sizeConstraints);
		float dx = c->width - widthBefore;
		UI::setX(m_Scene, c, c->x - dx, positionConstraints);
		resized = true;
	}
	else if (r->bordersHovered.right)
	{
		float dx = e.x - (c->x + c->width);
		UI::setWidth(m_Scene, c, c->width + dx, sizeConstraints);
		resized = true;
	}

	if (resized)
	{
		s_CurrentAction = Action::Resizing;
		if (r->onResized)
			r->onResized(UIEntityResizedEvent{ c->width, c->height });
	}
}

void SceneUIHandler::UpdateUIResizableComponentMouseButtonUp(Entity entity, UIResizableComponent* r, UIComponent* c, const MouseMovedEvent& e)
{
	r->bordersHovered = { false, false, false, false };

	if (Math::isInRange(e.beforeX, c->x - r->grabSize, c->x + c->width + r->grabSize))
	{
		if (r->bordersResizable.bottom && Math::isInRange(e.beforeY, c->y - r->grabSize, c->y + r->grabSize))
			r->bordersHovered.bottom = true;
		else if (r->bordersResizable.top && Math::isInRange(e.beforeY, c->y + c->height - r->grabSize, c->y + c->height + r->grabSize))
			r->bordersHovered.top = true;
	}

	if (Math::isInRange(e.beforeY, c->y - r->grabSize, c->y + c->height + r->grabSize))
	{
		if (r->bordersResizable.left && Math::isInRange(e.beforeX, c->x - r->grabSize, c->x + r->grabSize))
			r->bordersHovered.left = true;
		else if (r->bordersResizable.right && Math::isInRange(e.beforeX, c->x + c->width - r->grabSize, c->x + c->width + r->grabSize))
			r->bordersHovered.right = true;
	}

	if ((r->bordersHovered.bottom && r->bordersHovered.right) || (r->bordersHovered.top && r->bordersHovered.left))
		Window::setCursor(Cursors::ResizeDiagonalLeft);
	else if ((r->bordersHovered.bottom && r->bordersHovered.left) || (r->bordersHovered.top && r->bordersHovered.right))
		Window::setCursor(Cursors::ResizeDiagonalRight);
	else if (r->bordersHovered.bottom || r->bordersHovered.top)
		Window::setCursor(Cursors::ResizeVertical);
	else if (r->bordersHovered.left || r->bordersHovered.right)
		Window::setCursor(Cursors::ResizeHorizontal);

	if (r->bordersHovered)
	{
		if (m_Scene->HasComponent<UIHoverComponent>(entity))
			m_Scene->GetComponent<UIHoverComponent>(entity)->SetHovered(true);
		s_HoveredAction = Action::Resizing;
	}
}

void SceneUIHandler::UpdateUIMovableComponentMouseButtonUp(UIMovableComponent* m, UIComponent* c, const MouseMovedEvent& e)
{
	if (Sonic::Math::isInRange(e.beforeX, c->x, c->x + c->width) &&
		Sonic::Math::isInRange(e.beforeY, c->y, c->y + c->height))
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

		auto* parent = c->parent != 0 ? m_Scene->GetComponent<UIComponent>(c->parent) : nullptr;
		auto* constraints = m_Scene->HasComponent<UIPositionConstraintsComponent>(entity) ?
			m_Scene->GetComponent<UIPositionConstraintsComponent>(entity) : nullptr;

		float deltaX = e.x - c->width / 2 - c->x;
		float deltaY = e.y - c->height / 2 - c->y;

		UI::setX(m_Scene, c, c->x + deltaX, constraints);
		UI::setY(m_Scene, c, c->y + deltaY, constraints);

		if (m->onMoved)
			(*m->onMoved)(UIEntityMovedEvent{ deltaX, deltaY });
	}
}
