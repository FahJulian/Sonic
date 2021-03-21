#include "SceneSerializer.h"

#include <fstream>
#include <sstream>

#include "sonic/debug/Log.h"

namespace sonic
{
	namespace
	{
		Result<SceneData, SceneFileParsingError> _parseVersion_0_0_1(String& file)
		{
			SceneData data;

			size_t keyStartIndex = 0;
			size_t keyEndIndex = 0;
			size_t valueStartIndex = 0;
			size_t valueEndIndex = 0;
			while ((keyEndIndex = file.findFirst(':', valueEndIndex)) != file.getSize())
			{
				valueStartIndex = file.findFirstNotOf(' ', keyEndIndex + 1);
				if (file[valueStartIndex] == '{')
				{
					size_t level = 1;
					for (size_t i = valueStartIndex + 1; i < file.getSize(); i++)
					{
						if (file[i] == '{')
						{
							level++;
						}
						else if (file[i] == '}')
						{
							if (--level == 0)
							{
								valueEndIndex = i;
								break;
							}
						}
					}
				}
				else
				{
					valueEndIndex = file.findFirst(' ', valueStartIndex + 1);
				}


				String key = file.subString(keyStartIndex, keyEndIndex);
				String value = file.subString(valueStartIndex, valueEndIndex);

				if (size_t keySeperatorIndex = key.findFirst(' ');
					keySeperatorIndex != key.getSize())
				{
					String type = key.subString(0, keySeperatorIndex);
					if (!data.containsKey(type))
						data.insert(type, Map<String, String>());

					data[type].insert(key.subString(key.findFirstNotOf(' '), type.getSize() + 1), value);
				}

				keyStartIndex = file.findFirstNotOf(' ', valueEndIndex + 1);
			}

			return data;
		}

	} // namespace

	Result<SceneData, SceneFileParsingError> SceneSerializer::parse(const String& sceneFile)
	{
		String file;

		if (!(file << std::ifstream(sceneFile)))
			return SceneFileParsingError::COULD_NOT_READ_FILE;

		file.replaceAll('\n', ' ');
		file.replaceAll('\t', ' ');

		size_t typeKeyIndex = file.findFirst("type:");
		size_t typeStartIndex = file.findFirstNotOf(' ', typeKeyIndex + strlen("type:") + 1);
		String type = file.subString(typeStartIndex, file.findFirst(' ', typeStartIndex + 1));

		if (type != "scene")
			return SceneFileParsingError::INVALID_FILE_TYPE;

		size_t versionKeyIndex = file.findFirst("version:");
		size_t versionStartIndex = file.findFirstNotOf(' ', versionKeyIndex + strlen("version:") + 1);
		String version = file.subString(versionStartIndex, file.findFirst(' ', versionStartIndex + 1));

		if (version == "0.0.1")
			return _parseVersion_0_0_1(file);
		else
			return SceneFileParsingError::INVALID_FILE_VERSION;
	}

} // namespace sonic
