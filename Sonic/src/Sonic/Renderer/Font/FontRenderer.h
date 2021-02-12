#pragma once
#include "Sonic/Graphics/Color.h"
#include "Sonic/Graphics/Font.h"
#include "Sonic/Scene/Components/UIComponents.h"

namespace Sonic {

	class Scene;

	class FontRenderer
	{
	public:
		static void init();
		static void rebuffer(Scene* scene);
		static void render();
		static void setViewportSize(float width, float height);
		static void markDirty();
		static void destroy();

	private:
		static void drawCharacter(int index, float x, float y, float z, unsigned char c, const Font& font, const Color& color);
		static void drawString(int index, float x, float y, float z, const String& string, const Font& font, const Color& color);
		static void drawEntity(Scene* scene, Entity e, UITextComponent* t, int index);
	};

}
 