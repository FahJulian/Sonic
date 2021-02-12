#include <algorithm>
#include <fstream>
#include <json/json.hpp>
#include "Sonic/Util/IO/CursorLoader.h"
#include "Sonic/Debug/Log/Log.h"
#include "Cursors.h"

using namespace Sonic;


CursorSet::CursorSet(String folderPath)
	: m_Cursors(std::make_shared<std::unordered_map<String, Cursor>>())
{
	std::replace(folderPath.begin(), folderPath.end(), '/', '\\');
	String name = folderPath.substr(folderPath.find_last_of("\\") + 1);
	String jsonFile = folderPath + "\\" + name + ".json";
	std::ifstream file(jsonFile);

	JSON json;
	file >> json;

	for (auto& [name, value] : json["textures"].items())
	{
		String relativeTexturePath = value.get<String>();
		std::replace(relativeTexturePath.begin(), relativeTexturePath.end(), '/', '\\');
		String texturePath = folderPath + "\\" + relativeTexturePath;

		GLFWimage image;
		int offsetX, offsetY;
		image.pixels = Util::loadCursor(texturePath, image.width, image.height, offsetX, offsetY);

		Cursor cursor = glfwCreateCursor(&image, offsetX, offsetY);
		if (cursor == NULL)
			SONIC_LOG_ERROR(String(String("Could not load cursor ") + texturePath).c_str());

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

Cursor CursorSet::Get(const String& name) const
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

StandardCursors StandardCursors::s_StandardCursorSets[STANDARD_CURSOR_SETS_AMOUNT];
StandardCursors* StandardCursors::s_CurrentStandardCursorSet = &s_StandardCursorSets[static_cast<uint8_t>(Sonic::StandardCursors::Sets::Black)];

StandardCursors::StandardCursors(const String& folderPath)
	: CursorSet(folderPath)
{
	m_StandardCursors[static_cast<uint8_t>(StandardCursors::Arrow)] = Get("Arrow");
	m_StandardCursors[static_cast<uint8_t>(StandardCursors::Alternate)] = Get("Alternate");
	m_StandardCursors[static_cast<uint8_t>(StandardCursors::IBeam)] = Get("IBeam");
	m_StandardCursors[static_cast<uint8_t>(StandardCursors::Move)] = Get("Move");
	m_StandardCursors[static_cast<uint8_t>(StandardCursors::Crosshair)] = Get("Crosshair");
	m_StandardCursors[static_cast<uint8_t>(StandardCursors::ResizeHorizontal)] = Get("ResizeHorizontal");
	m_StandardCursors[static_cast<uint8_t>(StandardCursors::ResizeVertical)] = Get("ResizeVertical");
	m_StandardCursors[static_cast<uint8_t>(StandardCursors::ResizeDiagonalLeft)] = Get("ResizeDiagonalLeft");
	m_StandardCursors[static_cast<uint8_t>(StandardCursors::ResizeDiagonalRight)] = Get("ResizeDiagonalRight");
	m_StandardCursors[static_cast<uint8_t>(StandardCursors::Unavailable)] = Get("Unavailable");
	m_StandardCursors[static_cast<uint8_t>(StandardCursors::Pen)] = Get("Pen");
	m_StandardCursors[static_cast<uint8_t>(StandardCursors::Link)] = Get("Link");
	m_StandardCursors[static_cast<uint8_t>(StandardCursors::Help)] = Get("Help");
}

void StandardCursors::init()
{
	s_StandardCursorSets[static_cast<uint8_t>(StandardCursors::Sets::Black)] = StandardCursors(coreResourceDir() + "cursors/default_black");
	s_StandardCursorSets[static_cast<uint8_t>(StandardCursors::Sets::BlackShadow)] = StandardCursors(coreResourceDir() + "cursors/default_black_shadow");
	s_StandardCursorSets[static_cast<uint8_t>(StandardCursors::Sets::White)] = StandardCursors(coreResourceDir() + "cursors/default_white");
	s_StandardCursorSets[static_cast<uint8_t>(StandardCursors::Sets::WhiteShadow)] = StandardCursors(coreResourceDir() + "cursors/default_white_shadow");
}

Cursor StandardCursors::getCurrent(StandardCursor cursor)
{
	return s_CurrentStandardCursorSet->m_StandardCursors[static_cast<uint8_t>(cursor)];
}

void StandardCursors::setStandardCursorSet(StandardCursorSet cursorSet)
{
	s_CurrentStandardCursorSet = &s_StandardCursorSets[static_cast<uint8_t>(cursorSet)];
}
