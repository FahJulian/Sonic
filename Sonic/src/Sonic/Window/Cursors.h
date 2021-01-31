#pragma once
#include <unordered_map>
#include <gl/glew.h>
#include <glfw/glfw3.h>
#include "Sonic/Base.h"

namespace Sonic {
	
	using Cursor = GLFWcursor*;

	class CursorSet
	{
	public:
		CursorSet(std::string folderPath);
		~CursorSet();

		Cursor Get(const std::string& name) const;

	private:
		Ref<std::unordered_map<std::string, Cursor>> m_Cursors;
	};

	struct StandardCursors
	{
		static Cursor Arrow;
		static Cursor IBeam;
		static Cursor Crosshair;
		static Cursor ResizeHorizontal;
		static Cursor ResizeVertical;
		static Cursor ResizeDiagonalLeft;
		static Cursor ResizeDiagonalRight;

	private:
		static void init();

		friend class Window;
	};
}
