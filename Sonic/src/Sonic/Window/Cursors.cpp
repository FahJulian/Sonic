#include "Cursors.h"

namespace Sonic {

	Cursor Cursors::Arrow;
	Cursor Cursors::IBeam;
	Cursor Cursors::Crosshair;
	Cursor Cursors::Resize_EW;
	Cursor Cursors::Resize_NS;
	Cursor Cursors::Resize_NE;
	Cursor Cursors::Resize_SE;
	Cursor Cursors::Resize_NW;
	Cursor Cursors::Resize_SW;

	void Cursors::init()
	{
		Arrow = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		IBeam = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		Crosshair = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		Resize_EW = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		Resize_NS = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
		//Resize_NE = glfwCreateCursor(nullptr, 0, 0);
		//Resize_SE = glfwCreateCursor(nullptr, 0, 0);
		//Resize_NW = glfwCreateCursor(nullptr, 0, 0);
		//Resize_SW = glfwCreateCursor(nullptr, 0, 0);
	}

}
