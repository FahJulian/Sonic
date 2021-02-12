#pragma once
#include "Sonic/Base.h"
#include "Sonic/Scene/UI/UISize.h"
#include "Sonic/Scene/ECS/Entity.h"

namespace Sonic {

	struct UIComponent
	{
	private:
		using Mode = UISize::Mode;

		Entity parent;

		UISize x;
		UISize y;
		UISize width;
		UISize height;

		float zIndex;

		Ref<bool> uiRendererDirty = Ref<bool>(nullptr);
		Ref<bool> fontRendererDirty = Ref<bool>(nullptr);

	public:
		UIComponent(float x, float y, float width, float height, float zIndex = 0.0f)
			: parent(0), x({ Mode::Absolute, x }), y({ Mode::Absolute, y }), width({ Mode::Absolute, width }),
			height({ Mode::Absolute, height }), zIndex(zIndex)
		{
		}

		UIComponent(Mode mode, float x, float y, float width, float height, Entity parent = 0)
			: parent(parent), x({ mode, x }), y({ mode, y }), width({ mode, width }), height({ mode, height }), zIndex(0.0f)
		{
		}

		UIComponent(Mode mode, float x, float y, float width, float height, float zIndex, Entity parent = 0)
			: parent(parent), x({ mode, x }), y({ mode, y }), width({ mode, width }), height({ mode, height }), zIndex(zIndex)
		{
		}

		UIComponent(UISize x, UISize y, UISize width, UISize height, Entity parent = 0)
			: parent(parent), x(x), y(y), width(width), height(height), zIndex(0.0f)
		{
		}

		UIComponent(UISize x, UISize y, UISize width, UISize height, float zIndex, Entity parent = 0)
			: parent(parent), x(x), y(y), width(width), height(height), zIndex(zIndex)
		{
		}

		float GetX() const { return x.absoluteValue; }
		float GetY() const { return y.absoluteValue; }
		float GetWidth() const { return width.absoluteValue; }
		float GetHeight() const { return height.absoluteValue; }
		float GetZIndex() const { return zIndex; }

		friend class SceneUIHandler;
	};

}
