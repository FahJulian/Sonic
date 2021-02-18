#pragma once
#include <vector>
#include "Sonic/Base.h"
#include "Sonic/Scene/UI/UISize.h"
#include "Sonic/Scene/ECS/Entity.h"

namespace Sonic {

	struct UIComponent
	{
		using Mode = UISize::Mode;

		Entity parent;

		union
		{
			float x;
			UISize xPattern;
		};

		union
		{
			float y;
			UISize yPattern;
		};

		union
		{
			float width;
			UISize widthPattern;
		};

		union
		{
			float height;
			UISize heightPattern;
		};

		float zIndex;

		Ref<bool> uiRendererDirty = Ref<bool>(nullptr);
		Ref<bool> fontRendererDirty = Ref<bool>(nullptr);

		Ref<std::vector<Entity>> childs = createRef<std::vector<Entity>>();

		UIComponent(float x, float y, float width, float height, float zIndex = 0.0f)
			: parent(0), xPattern(Mode::Absolute, x), yPattern(Mode::Absolute, y), widthPattern(Mode::Absolute, width),
			heightPattern(Mode::Absolute, height), zIndex(zIndex)
		{
		}

		UIComponent(Mode mode, float x, float y, float width, float height, Entity parent = 0)
			: parent(parent), xPattern(mode, x), yPattern(mode, y), widthPattern(mode, width), heightPattern(mode, height), zIndex(0.0f)
		{
		}

		UIComponent(Mode mode, float x, float y, float width, float height, float zIndex, Entity parent = 0)
			: parent(parent), xPattern(mode, x), yPattern(mode, y), widthPattern(mode, width), heightPattern(mode, height), zIndex(zIndex)
		{
		}

		UIComponent(UISize x, UISize y, UISize width, UISize height, Entity parent = 0)
			: parent(parent), xPattern(x), yPattern(y), widthPattern(width), heightPattern(height), zIndex(0.0f)
		{
		}

		UIComponent(UISize x, UISize y, UISize width, UISize height, float zIndex, Entity parent = 0)
			: parent(parent), xPattern(x), yPattern(y), widthPattern(width), heightPattern(height), zIndex(zIndex)
		{
		}

		float GetX() const { return x; }
		float GetY() const { return y; }
		float GetWidth() const { return width; }
		float GetHeight() const { return height; }
		float GetZIndex() const { return zIndex; }
	};

}
