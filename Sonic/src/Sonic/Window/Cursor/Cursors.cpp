#include <algorithm>
#include <fstream>
#include <json/json.hpp>
#include "Sonic/Util/CursorLoader.h"
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
			int offsetX, offsetY;
			image.pixels = Util::loadCursor(texturePath, image.width, image.height, offsetX, offsetY);

			Cursor cursor = glfwCreateCursor(&image, offsetX, offsetY);
			if (cursor == NULL)
				SONIC_LOG_ERROR(std::string(std::string("Could not load cursor ") + texturePath).c_str());

			m_Cursors->insert({ name, cursor });

			delete[] image.pixels;
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

	DefaultCursorsSet DefaultCursorsSet::s_StandardCursorSets[STANDARD_CURSOR_SETS_AMOUNT];
	DefaultCursorsSet* DefaultCursorsSet::s_CurrentStandardCursorSet = &s_StandardCursorSets[static_cast<uint8_t>(Sonic::DefaultCursorSets::Black)];

	DefaultCursorsSet::DefaultCursorsSet(const std::string& folderPath)
		: CursorSet(folderPath)
	{
		m_StandardCursors[static_cast<uint8_t>(DefaultCursors::Arrow)] = Get("Arrow");
		m_StandardCursors[static_cast<uint8_t>(DefaultCursors::Alternate)] = Get("Alternate");
		m_StandardCursors[static_cast<uint8_t>(DefaultCursors::IBeam)] = Get("IBeam");
		m_StandardCursors[static_cast<uint8_t>(DefaultCursors::Crosshair)] = Get("Crosshair");
		m_StandardCursors[static_cast<uint8_t>(DefaultCursors::ResizeHorizontal)] = Get("ResizeHorizontal");
		m_StandardCursors[static_cast<uint8_t>(DefaultCursors::ResizeVertical)] = Get("ResizeVertical");
		m_StandardCursors[static_cast<uint8_t>(DefaultCursors::ResizeDiagonalLeft)] = Get("ResizeDiagonalLeft");
		m_StandardCursors[static_cast<uint8_t>(DefaultCursors::ResizeDiagonalRight)] = Get("ResizeDiagonalRight");
		m_StandardCursors[static_cast<uint8_t>(DefaultCursors::Unavailable)] = Get("Unavailable");
		m_StandardCursors[static_cast<uint8_t>(DefaultCursors::Pen)] = Get("Pen");
		m_StandardCursors[static_cast<uint8_t>(DefaultCursors::Link)] = Get("Link");
		m_StandardCursors[static_cast<uint8_t>(DefaultCursors::Help)] = Get("Help");
	}

	void DefaultCursorsSet::initStandardCursorSets()
	{
		s_StandardCursorSets[static_cast<uint8_t>(DefaultCursorSets::Black)] = DefaultCursorsSet(SONIC_RESOURCE_DIR + "cursors/default_black");
		s_StandardCursorSets[static_cast<uint8_t>(DefaultCursorSets::BlackShadow)] = DefaultCursorsSet(SONIC_RESOURCE_DIR + "cursors/default_black_shadow");
		s_StandardCursorSets[static_cast<uint8_t>(DefaultCursorSets::White)] = DefaultCursorsSet(SONIC_RESOURCE_DIR + "cursors/default_white");
		s_StandardCursorSets[static_cast<uint8_t>(DefaultCursorSets::WhiteShadow)] = DefaultCursorsSet(SONIC_RESOURCE_DIR + "cursors/default_white_shadow");
	}

	Cursor DefaultCursorsSet::getCurrent(DefaultCursors cursor)
	{
		return s_CurrentStandardCursorSet->m_StandardCursors[static_cast<uint8_t>(cursor)];
	}

	void DefaultCursorsSet::setStandardCursorSet(DefaultCursorSets cursorSet)
	{
		s_CurrentStandardCursorSet = &s_StandardCursorSets[static_cast<uint8_t>(cursorSet)];
	}

}
