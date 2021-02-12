#pragma once
#include <string>
#include "Sonic/Graphics/Color.h"
#include "Sonic/Graphics/Font.h"

namespace Sonic {

	struct UITextComponent
	{
	private:
		Font font;
		Color color;
		std::string text;

	public:
		Ref<bool> dirty;

		UITextComponent(Font font, const Color& color, const std::string& text)
			: font(font), color(color), text(text), dirty(new bool(true))
		{
		}

		const Font& GetFont() const { return font; }
		const Color& GetColor() const { return color; }
		const std::string& GetText() const { return text; }

		void SetFont(const Font& newFont) { font = newFont; *dirty = true; }
		void SetColor(const Color& newColor) { color = newColor; *dirty = true; }
		void SetText(const std::string& newText) { text = newText; *dirty = true; }
	};

}
