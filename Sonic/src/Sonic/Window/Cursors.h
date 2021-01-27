#pragma once
#include <gl/glew.h>
#include <glfw/glfw3.h>

namespace Sonic {
	
	using Cursor = GLFWcursor*;

	class Cursors
	{
	public:
		static Cursor Arrow;
		static Cursor IBeam;
		static Cursor Crosshair;
		static Cursor Resize_EW;
		static Cursor Resize_NS;
		static Cursor Resize_NE;
		static Cursor Resize_SE;
		static Cursor Resize_NW;
		static Cursor Resize_SW;

		static void init();
	};

}
