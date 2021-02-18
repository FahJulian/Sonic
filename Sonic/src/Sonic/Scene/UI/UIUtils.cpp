#include "Sonic/Util/Math/Math.h"
#include "Sonic/Debug/Log/Log.h"
#include "Sonic/Window/Window.h"
#include "Sonic/Scene/Scene.h"
#include "UIUtils.h"

using namespace Sonic;


static float toAbsoluteX(UIComponent* c, UIComponent* parent, float value);
static float toAbsoluteY(UIComponent* c, UIComponent* parent, float value);
static float toAbsoluteWidth(UIComponent* c, UIComponent* parent, float value);
static float toAbsoluteHeight(UIComponent* c, UIComponent* parent, float value);

static float toRelativeX(UIComponent* c, UIComponent* parent, float value);
static float toRelativeY(UIComponent* c, UIComponent* parent, float value);
static float toRelativeWidth(UIComponent* c, UIComponent* parent, float value);
static float toRelativeHeight(UIComponent* c, UIComponent* parent, float value);


static float toAbsoluteX(UIComponent* c, UIComponent* parent, float value)
{
	switch (c->xPattern.mode)
	{
	case UISize::Mode::Absolute: return value;
	case UISize::Mode::WindowCenter: return (Window::getWidth() - toAbsoluteWidth(c, parent, c->widthPattern.relativeValue)) / 2.0f - toAbsoluteWidth(c, parent, value);
	case UISize::Mode::RelativeToWindow: return value * Window::getWidth();
	case UISize::Mode::RelativeToEntity: return parent->x + value * parent->width;
	default:
		SONIC_LOG_WARN("Illegal UISize::Mode for x value");
		return 0.0f;
	}
}

static float toAbsoluteY(UIComponent* c, UIComponent* parent, float value)
{
	switch (c->yPattern.mode)
	{
	case UISize::Mode::Absolute: return value;
	case UISize::Mode::WindowCenter: return (Window::getHeight() - toAbsoluteHeight(c, parent, c->heightPattern.relativeValue)) / 2.0f - toAbsoluteHeight(c, parent, value);
	case UISize::Mode::RelativeToWindow: return value * Window::getHeight();
	case UISize::Mode::RelativeToEntity: return parent->y + value * parent->height;
	default:
		SONIC_LOG_WARN("Illegal UISize::Mode for y value");
		return 0.0f;
	}
}

static float toAbsoluteWidth(UIComponent* c, UIComponent* parent, float value)
{
	switch (c->widthPattern.mode)
	{
	case UISize::Mode::Absolute: return value;
	case UISize::Mode::RelativeToWindow: return value * Window::getWidth();
	case UISize::Mode::RelativeToEntity: return value * parent->width;
	default:
		SONIC_LOG_WARN("Illegal UISize::Mode for width value");
		return 0.0f;
	}
}

static float toAbsoluteHeight(UIComponent* c, UIComponent* parent, float value)
{
	switch (c->heightPattern.mode)
	{
	case UISize::Mode::Absolute: return value;
	case UISize::Mode::RelativeToWindow: return value * Window::getHeight();
	case UISize::Mode::RelativeToEntity: return value * parent->height;
	default:
		SONIC_LOG_WARN("Illegal UISize::Mode for height value");
		return 0.0f;
	}
}

static float toRelativeX(UIComponent* c, UIComponent* parent, float value)
{
	switch (c->xPattern.mode)
	{
	case UISize::Mode::Absolute: return value;
	case UISize::Mode::WindowCenter: return toRelativeWidth(c, parent, value - (Window::getWidth() - toAbsoluteWidth(c, parent, c->widthPattern.relativeValue)) / 2.0f);
	case UISize::Mode::RelativeToWindow: return value / Window::getWidth();
	case UISize::Mode::RelativeToEntity: return (value - parent->x) / parent->width;
	default:
		SONIC_LOG_WARN("Illegal UISize::Mode for x value");
		return 0.0f;
	}
}

static float toRelativeY(UIComponent* c, UIComponent* parent, float value)
{
	switch (c->yPattern.mode)
	{
	case UISize::Mode::Absolute: return value;
	case UISize::Mode::WindowCenter: return toRelativeHeight(c, parent, value - (Window::getHeight() - toAbsoluteHeight(c, parent, c->heightPattern.relativeValue)) / 2.0f);
	case UISize::Mode::RelativeToWindow: return value / Window::getHeight();
	case UISize::Mode::RelativeToEntity: return (value - parent->y) / parent->height;
	default:
		SONIC_LOG_WARN("Illegal UISize::Mode for y value");
		return 0.0f;
	}
}

static float toRelativeWidth(UIComponent* c, UIComponent* parent, float value)
{
	switch (c->widthPattern.mode)
	{
	case UISize::Mode::Absolute: return value;
	case UISize::Mode::RelativeToWindow: return value / Window::getWidth();
	case UISize::Mode::RelativeToEntity: return value / parent->width;
	default:
		SONIC_LOG_WARN("Illegal UISize::Mode for width value");
		return 0.0f;
	}
}

static float toRelativeHeight(UIComponent* c, UIComponent* parent, float value)
{
	switch (c->heightPattern.mode)
	{
	case UISize::Mode::Absolute: return value;
	case UISize::Mode::RelativeToWindow: return value / Window::getHeight();
	case UISize::Mode::RelativeToEntity: return value / parent->height;
	default:
		SONIC_LOG_WARN("Illegal UISize::Mode for height value");
		return 0.0f;
	}
}


static void markDirty(UIComponent* c)
{
	if (c->uiRendererDirty)
		*c->uiRendererDirty = true;
	if (c->fontRendererDirty)
		*c->fontRendererDirty = true;
}


static void setXAndUpdateChilds(Scene* scene, UIComponent* c, float absoluteValue)
{
	float delta = absoluteValue - c->xPattern.absoluteValue;
	c->xPattern.absoluteValue = absoluteValue;
	
	for (Entity child : *c->childs)
	{
		if (auto childComponent = scene->GetComponent<UIComponent>(child);
			childComponent->xPattern.mode == UISize::Mode::RelativeToEntity)
		{
			setXAndUpdateChilds(scene, childComponent, childComponent->x + delta);
		}
	}

	markDirty(c);
}

static void setYAndUpdateChilds(Scene* scene, UIComponent* c, float absoluteValue)
{
	float delta = absoluteValue - c->yPattern.absoluteValue;
	c->yPattern.absoluteValue = absoluteValue;

	for (Entity child : *c->childs)
	{
		if (auto childComponent = scene->GetComponent<UIComponent>(child);
			childComponent->yPattern.mode == UISize::Mode::RelativeToEntity)
		{
			setYAndUpdateChilds(scene, childComponent, childComponent->y + delta);
		}
	}

	markDirty(c);
}

static void setWidthAndUpdateChilds(Scene* scene, UIComponent* c, float absoluteValue)
{
	float delta = absoluteValue - c->widthPattern.absoluteValue;
	c->widthPattern.absoluteValue = absoluteValue;

	for (Entity child : *c->childs)
	{
		if (auto childComponent = scene->GetComponent<UIComponent>(child);
			childComponent->widthPattern.mode == UISize::Mode::RelativeToEntity)
		{
			childComponent->x += childComponent->xPattern.relativeValue * delta;
			setWidthAndUpdateChilds(scene, childComponent, childComponent->width + childComponent->widthPattern.relativeValue * delta);
		}
	}

	markDirty(c);
}

static void setHeightAndUpdateChilds(Scene* scene, UIComponent* c, float absoluteValue)
{
	float delta = absoluteValue - c->heightPattern.absoluteValue;
	c->heightPattern.absoluteValue = absoluteValue;

	for (Entity child : *c->childs)
	{
		if (auto childComponent = scene->GetComponent<UIComponent>(child);
			childComponent->heightPattern.mode == UISize::Mode::RelativeToEntity)
		{
			childComponent->y += childComponent->yPattern.relativeValue * delta;
			setHeightAndUpdateChilds(scene, childComponent, childComponent->height + childComponent->heightPattern.relativeValue * delta);
		}
	}

	markDirty(c);
}


UIComponent* UI::getParent(Scene* scene, UIComponent* c)
{
	return c->parent != 0 ? scene->GetComponent<UIComponent>(c->parent) : nullptr;
}


void UI::setX(Scene* scene, UIComponent* c, float value, UIPositionConstraintsComponent* constraints)
{
	UIComponent* parent = getParent(scene, c);

	if (constraints == nullptr)
	{
		c->xPattern.relativeValue = toRelativeX(c, parent, value);
	}
	else
	{
		c->xPattern.relativeValue = Math::clamp(toRelativeX(c, parent, value), constraints->minX, constraints->maxX); 
		value = toAbsoluteX(c, parent, c->xPattern.relativeValue);
	}
	
	setXAndUpdateChilds(scene, c, value);
}

void UI::setY(Scene* scene, UIComponent* c, float value, UIPositionConstraintsComponent* constraints)
{
	UIComponent* parent = getParent(scene, c);

	if (constraints == nullptr)
	{
		c->yPattern.relativeValue = toRelativeY(c, parent, value);
	}
	else
	{
		c->yPattern.relativeValue = Math::clamp(toRelativeY(c, parent, value), constraints->minY, constraints->maxY);
		value = toAbsoluteY(c, parent, c->yPattern.relativeValue);
	}

	setYAndUpdateChilds(scene, c, value);
}

void UI::setWidth(Scene* scene, UIComponent* c, float value, UISizeConstraintsComponent* constraints)
{
	UIComponent* parent = getParent(scene, c);

	if (constraints == nullptr)
	{
		c->widthPattern.relativeValue = toRelativeWidth(c, parent, value);
	}
	else
	{
		c->widthPattern.relativeValue = Math::clamp(toRelativeWidth(c, parent, value), constraints->minWidth, constraints->maxWidth);
		value = toAbsoluteWidth(c, parent, c->widthPattern.relativeValue);
	}

	setWidthAndUpdateChilds(scene, c, value);
}

void UI::setHeight(Scene* scene, UIComponent* c, float value, UISizeConstraintsComponent* constraints)
{
	UIComponent* parent = getParent(scene, c);

	if (constraints == nullptr)
	{
		c->heightPattern.relativeValue = toRelativeHeight(c, parent, value);
	}
	else
	{
		c->heightPattern.relativeValue = Math::clamp(toRelativeHeight(c, parent, value), constraints->minHeight, constraints->maxHeight);
		value = toAbsoluteHeight(c, parent, c->heightPattern.relativeValue);
	}

	setHeightAndUpdateChilds(scene, c, value);
}


void UI::setX(Scene* scene, Entity e, UIComponent* c, float value)
{
	setX(scene, c, value,
		scene->HasComponent<UIPositionConstraintsComponent>(e) ? scene->GetComponent<UIPositionConstraintsComponent>(e) : nullptr);
}

void UI::setY(Scene* scene, Entity e, UIComponent* c, float value)
{
	setY(scene, c, value,
		scene->HasComponent<UIPositionConstraintsComponent>(e) ? scene->GetComponent<UIPositionConstraintsComponent>(e) : nullptr);
}

void UI::setWidth(Scene* scene, Entity e, UIComponent* c, float value)
{
	setWidth(scene, c, value,
		scene->HasComponent<UISizeConstraintsComponent>(e) ? scene->GetComponent<UISizeConstraintsComponent>(e) : nullptr);
}

void UI::setHeight(Scene* scene, Entity e, UIComponent* c, float value)
{
	setHeight(scene, c, value,
		scene->HasComponent<UISizeConstraintsComponent>(e) ? scene->GetComponent<UISizeConstraintsComponent>(e) : nullptr);
}


void UI::setXRelative(Scene* scene, UIComponent* c, float value, UIPositionConstraintsComponent* constraints)
{
	if (constraints != nullptr)
		value = Math::clamp(value, constraints->minX, constraints->maxX);
	c->xPattern.relativeValue = value;

	setXAndUpdateChilds(scene, c, toAbsoluteX(c, getParent(scene, c), value));
}

void UI::setYRelative(Scene* scene, UIComponent* c, float value, UIPositionConstraintsComponent* constraints)
{
	if (constraints != nullptr)
		value = Math::clamp(value, constraints->minY, constraints->maxY);
	c->yPattern.relativeValue = value;

	setYAndUpdateChilds(scene, c, toAbsoluteY(c, getParent(scene, c), value));
}

void UI::setWidthRelative(Scene* scene, UIComponent* c, float value, UISizeConstraintsComponent* constraints)
{
	if (constraints != nullptr)
		value = Math::clamp(value, constraints->minWidth, constraints->maxWidth);
	c->widthPattern.relativeValue = value;

	setWidthAndUpdateChilds(scene, c, toAbsoluteWidth(c, getParent(scene, c), value));
}

void UI::setHeightRelative(Scene* scene, UIComponent* c, float value, UISizeConstraintsComponent* constraints)
{
	if (constraints != nullptr)
		value = Math::clamp(value, constraints->minHeight, constraints->maxHeight);
	c->heightPattern.relativeValue = value;

	setHeightAndUpdateChilds(scene, c, toAbsoluteHeight(c, getParent(scene, c), value));
}


void UI::setXRelative(Scene* scene, Entity e, UIComponent* c, float value)
{
	setXRelative(scene, c, value,
		scene->HasComponent<UIPositionConstraintsComponent>(e) ? scene->GetComponent<UIPositionConstraintsComponent>(e) : nullptr);
}

void UI::setYRelative(Scene* scene, Entity e, UIComponent* c, float value)
{
	setYRelative(scene, c, value,
		scene->HasComponent<UIPositionConstraintsComponent>(e) ? scene->GetComponent<UIPositionConstraintsComponent>(e) : nullptr);
}

void UI::setWidthRelative(Scene* scene, Entity e, UIComponent* c, float value)
{
	setWidthRelative(scene, c, value,
		scene->HasComponent<UISizeConstraintsComponent>(e) ? scene->GetComponent<UISizeConstraintsComponent>(e) : nullptr);
}

void UI::setHeightRelative(Scene* scene, Entity e, UIComponent* c, float value)
{
	setHeightRelative(scene, c, value,
		scene->HasComponent<UISizeConstraintsComponent>(e) ? scene->GetComponent<UISizeConstraintsComponent>(e) : nullptr);
}


void UI::recalculate(Scene* scene, UIComponent* c)
{
	setXAndUpdateChilds(scene, c, toAbsoluteX(c, getParent(scene, c), c->xPattern.relativeValue));
	setYAndUpdateChilds(scene, c, toAbsoluteY(c, getParent(scene, c), c->yPattern.relativeValue));
	setWidthAndUpdateChilds(scene, c, toAbsoluteWidth(c, getParent(scene, c), c->widthPattern.relativeValue));
	setHeightAndUpdateChilds(scene, c, toAbsoluteHeight(c, getParent(scene, c), c->heightPattern.relativeValue));
}


void UI::checkPosition(Scene* scene, UIComponent* c, UIPositionConstraintsComponent* constraints)
{
	float newX = Math::clamp(c->x, constraints->minX, constraints->maxX);
	if (newX != c->x)
	{
		c->xPattern.relativeValue = newX;
		setXAndUpdateChilds(scene, c, toAbsoluteX(c, getParent(scene, c), newX));
	}

	float newY = Math::clamp(c->y, constraints->minY, constraints->maxY);
	if (newY != c->y)
	{
		c->yPattern.relativeValue = newY;
		setYAndUpdateChilds(scene, c, toAbsoluteY(c, getParent(scene, c), newY));
	}
}

void UI::checkSize(Scene* scene, UIComponent* c, UISizeConstraintsComponent* constraints)
{
	float newWidth = Math::clamp(c->width, constraints->minWidth, constraints->maxWidth);
	if (newWidth != c->width)
	{
		c->widthPattern.relativeValue = newWidth;
		setWidthAndUpdateChilds(scene, c, toAbsoluteWidth(c, getParent(scene, c), newWidth));
	}

	float newHeight = Math::clamp(c->height, constraints->minHeight, constraints->maxHeight);
	if (newHeight != c->height)
	{
		c->heightPattern.relativeValue = newHeight;
		setHeightAndUpdateChilds(scene, c, toAbsoluteHeight(c, getParent(scene, c), newHeight));
	}
}
