#pragma once
#include <unordered_map>
#include <gl/glew.h>
#include <glfw/glfw3.h>
#include "Sonic/Base.h"

namespace Sonic {
	
	using Cursor = GLFWcursor*;
	using StandardCursor = uint8_t;
	using StandardCursorSet = uint8_t;

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

		friend class StandardCursors;
	};

	static const int STANDARD_CURSOR_SETS_AMOUNT = 4;
	static const int STANDARD_CURSORS_AMOUNT = 12;

	class StandardCursors : public CursorSet
	{
	public:
		enum : StandardCursor
		{
			Arrow,
			Alternate,
			IBeam,
			Move,
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

		enum class Sets : StandardCursorSet
		{
			Black,
			BlackShadow,
			White,
			WhiteShadow
		};

	private:
		StandardCursors() = default;
		StandardCursors(const std::string& folderPath);

		Cursor m_StandardCursors[STANDARD_CURSORS_AMOUNT];

		static void init();

		static Cursor getCurrent(StandardCursor cursor);

		static void setStandardCursorSet(StandardCursorSet cursorSet);

		static StandardCursors s_StandardCursorSets[Sonic::STANDARD_CURSOR_SETS_AMOUNT];
		static StandardCursors* s_CurrentStandardCursorSet;

		friend class Window;
	};
}
