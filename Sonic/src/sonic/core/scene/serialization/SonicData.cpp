#include "SonicData.h"

#include <fstream>

#include "sonic/debug/Log.h"

namespace sonic
{
    namespace
    {
        void _parseVersion0_0_1(SonicData* data, String& content)
        {

        }
    } // namespace

	SonicData::Element::~Element()
    {
        if (isDictionary())
            mDictionary.~Dictionary();
        else if (isList())
            mList.~List();
        else if (isString())
            mString.~String();
    }

    SonicData::SonicData(const String& filePath)
    {
        std::ifstream file = std::ifstream(filePath.getData(), std::ifstream::end);
        size_t size = file.tellg();
        file.seekg(std::ifstream::beg);

        String content;
        file.read(content.getData(), size);
        file.close();

        String version;
        content = content.trim();
        if (!content.startsWith("#version") && !content.startsWith("# version"))
            return;

        size_t versionBeginIndex = content.findFirstNotOf(' ', strlen("#version"));
        version = content.subString(versionBeginIndex, content.findFirstOf('n')).trim();

        if (version == "0.0.1")
            _parseVersion0_0_1(this, content);
        else
            Log::warn("Could not parse sonic file ", filePath, ": unknown version");
    }

} // namespace
