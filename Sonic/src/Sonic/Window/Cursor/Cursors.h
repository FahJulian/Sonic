#pragma once
#include <unordered_map>
#include <gl/glew.h>
#include <glfw/glfw3.h>
#include "Sonic/Base.h"

namespace Sonic {
	
	using Cursor = GLFWcursor*;

	class CursorSet
	{
	private:
		CursorSet() = default;

	public:
		CursorSet(std::string folderPath);
		~CursorSet();

		Cursor Get(const std::string& name) const;

	private:
		Ref<std::unordered_map<std::string, Cursor>> m_Cursors;

		friend class DefaultCursorsSet;
	};

	enum class DefaultCursorSets : uint8_t
	{
		Black, BlackShadow, White, WhiteShadow
	};

	enum class DefaultCursors : uint8_t
	{
		Arrow,
		Alternate,
		IBeam,
		Crosshair,
		ResizeHorizontal,
		ResizeVertical,
		ResizeDiagonalLeft,
		ResizeDiagonalRight,
		Unavailable,
		Pen,
		Link,
		Help
	};

	static const int STANDARD_CURSOR_SETS_AMOUNT = 4;
	static const int STANDARD_CURSORS_AMOUNT = 12;

	class DefaultCursorsSet : public CursorSet
	{
	private:
		DefaultCursorsSet() = default;
		DefaultCursorsSet(const std::string& folderPath);

		Cursor m_StandardCursors[STANDARD_CURSORS_AMOUNT];

		static void initStandardCursorSets();

		static Cursor getCurrent(DefaultCursors cursor);

		static void setStandardCursorSet(DefaultCursorSets cursorSet);

		static Sonic::DefaultCursorsSet s_StandardCursorSets[Sonic::STANDARD_CURSOR_SETS_AMOUNT];
		static Sonic::DefaultCursorsSet* s_CurrentStandardCursorSet;

		friend class Window;
	};
}
