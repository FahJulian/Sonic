#include <algorithm>
#include <fstream>
#include <stb/stb_image.h>
#include <json/json.hpp>
#include "Sonic/Base.h"
#include "Sonic/Log/Log.h"
#include "Cursors.h"


namespace Sonic {

	CursorSet::CursorSet(std::string folderPath)
		: m_Cursors(std::make_shared<std::unordered_map<std::string, Cursor>>())
	{
		std::replace(folderPath.begin(), folderPath.end(), '/', '\\');
		std::string name = folderPath.substr(folderPath.find_last_of("\\") + 1);
		std::string jsonFile = folderPath + "\\" + name + ".json";
		std::ifstream file(jsonFile);

		JSON json;
		file >> json;

		for (auto& [name, value] : json["textures"].items())
		{
			std::string relativeTexturePath = value.get<std::string>();
			std::replace(relativeTexturePath.begin(), relativeTexturePath.end(), '/', '\\');
			std::string texturePath = folderPath + "\\" + relativeTexturePath;

			GLFWimage image;
			image.pixels = stbi_load(texturePath.c_str(), &image.width, &image.height, NULL, 4);

			Cursor cursor = glfwCreateCursor(&image, 0, 0);
			if (cursor == NULL)
				SONIC_LOG_ERROR(std::string(std::string("Could not load cursor ") + texturePath).c_str());

			m_Cursors->insert({ name, cursor });

			stbi_image_free(image.pixels);
		}
	}

	CursorSet::~CursorSet()
	{
		if (m_Cursors.use_count() == 1)
			for (auto [_, cursor] : *m_Cursors)
				glfwDestroyCursor(cursor);
	}

	Cursor CursorSet::Get(const std::string& name) const
	{
#ifdef SONIC_DEBUG
		if (m_Cursors->find(name) == m_Cursors->end())
		{
			SONIC_LOG_DEBUG("Cursor does not exist. Check spelling and lower/uppercase");
			return nullptr;
		}
#endif

		return m_Cursors->at(name);
	}

	Cursor StandardCursors::Arrow;
	Cursor StandardCursors::IBeam;
	Cursor StandardCursors::Crosshair;
	Cursor StandardCursors::ResizeHorizontal;
	Cursor StandardCursors::ResizeVertical;
	Cursor StandardCursors::ResizeDiagonalLeft;
	Cursor StandardCursors::ResizeDiagonalRight;

	void StandardCursors::init()
	{
		static CursorSet cursors = CursorSet(SONIC_RESOURCE_DIR + "cursors/ml_blau");

		Arrow = cursors.Get("Arrow");
		IBeam = cursors.Get("IBeam");
		Crosshair = cursors.Get("Crosshair");
		ResizeHorizontal = cursors.Get("ResizeHorizontal");
		ResizeVertical = cursors.Get("ResizeVertical");
		ResizeDiagonalLeft = cursors.Get("ResizeDiagonalLeft");
		ResizeDiagonalRight = cursors.Get("ResizeDiagonalRight");
	}

}
