#pragma once
#include <string>
#include "Sonic/Base.h"
#include "Sonic/Graphics/Color.h"
#include "Sonic/Graphics/Font.h"

namespace Sonic {

	struct UITextComponent
	{
	private:
		Font font;
		Color color;
		String text;

	public:
		Ref<bool> dirty;

		UITextComponent(Font font, const Color& color, const String& text)
			: font(font), color(color), text(text), dirty(new bool(true))
		{
		}

		const Font& GetFont() const { return font; }
		const Color& GetColor() const { return color; }
		const String& GetText() const { return text; }

		void SetFont(const Font& newFont) { font = newFont; *dirty = true; }
		void SetColor(const Color& newColor) { color = newColor; *dirty = true; }
		void SetText(const String& newText) { text = newText; *dirty = true; }
	};

}
