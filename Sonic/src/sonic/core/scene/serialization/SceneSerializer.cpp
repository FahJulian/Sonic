#include "SceneSerializer.h"

#include <fstream>
#include <sstream>

#include "sonic/debug/Log.h"

namespace sonic
{
	namespace
	{
		size_t _findValueEndIndex_0_0_1(String& file, size_t valueStartIndex)
		{
			if (file[valueStartIndex] == '{')
			{
				size_t level = 1;
				for (size_t i = valueStartIndex + 1; i < file.getSize(); i++)
				{
					if (file[i] == '{')
						level++;
					else if (file[i] == '}' && (--level) == 0)
						return i;
				}

				return file.getSize();
			}
			else
			{
				return file.findFirstOf(' ', valueStartIndex + 1);
			}
		}

		Result<SceneFileData, SceneFileParsingError> _parseVersion0_0_1(String sceneName, String version, String& file)
		{
			Log::log(Log::INFO, "Parsing scene file for scene ", sceneName, "...");

			SceneFileData data = { };
			data.name = sceneName;
			data.version = version;

			size_t keyStartIndex = 0;
			size_t keyEndIndex = 0;
			while ((keyEndIndex = file.findFirstOf(": ", keyStartIndex + 1)) != file.getSize())
			{
				size_t valueStartIndex = file.findFirstNotOf(' ', keyEndIndex + strlen(": "));
				size_t valueEndIndex = _findValueEndIndex_0_0_1(file, valueStartIndex);

				if (valueEndIndex == file.getSize())
					return SceneFileParsingError::SYNTAX_ERROR_REACHED_EOF;

				String key = file.subString(keyStartIndex, keyEndIndex);
				String value = file.subString(valueStartIndex, valueEndIndex);

				if (size_t keySeperatorIndex = key.findFirstOf(' ', 1);
					keySeperatorIndex != key.getSize())
				{
					String type = key.subString(0, keySeperatorIndex);
					if (!data.data.containsKey(type))
						data.data.insert(type, Map<String, String>());

					data.data[type].insert(key.subString(key.findFirstNotOf(' '), type.getSize() + 1), value);
				}

				keyStartIndex = file.findFirstNotOf(' ', valueEndIndex + 1);
			}

			Log::log(Log::INFO, "Done.");

			return data;
		}

		void _generateCodeVersion0_0_1(const SceneFileData& data)
		{
			Log::log(Log::INFO, "Generating code for scene ", data.name, "...");

			String code;

			code += "#include \"sonic/core/scene/Scene.h\"\n\n";
			code += "#include \"SceneData.hpp\"\n\n";

			code += "namespace sonic::client\n{\n";

			code += "    class " + data.name + " : public Scene\n    {\n";

			code += "    public:\n";

			code += "        TestScene()\n";
			code += "            : Scene(\"" + data.name + "\", &mData)\n";
			code += "        {\n        }\n\n";

			code += "        void initComponentPools() override\n";
			code += "        {\n";

			// TODO: Insert all needed component pools

			code += "        }\n\n";

			code += "        void initSystems() override\n";
			code += "        {\n";

			// TODO: Insert all needed systems

			code += "        }\n\n";

			code += "        void updateComponents(float deltaTime) override\n";
			code += "        {\n";

			// TODO: Insert all component pools

			code += "        }\n\n";

			code += "        void updateSystems(float deltaTime) override\n";
			code += "        {\n";

			// TODO: Insert all systems

			code += "        }\n\n";

			code += "        SceneData mData;\n";

			// TODO Insert all componentpools and systems

			code += "    };\n\n";
			code += "} // namespace sonic::client\n";

			std::ofstream file = std::ofstream("C:\\Users\\fahju\\AppData\\Roaming\\Sonic\\tmp\\sandbox\\sandbox.cpp");
			file.write(code.getData(), code.getSize());
			file.close();

			Log::log(Log::INFO, "Done.");
		}

	} // namespace

	Result<SceneFileData, SceneFileParsingError> SceneSerializer::parse(const String& filePath)
	{
		String file;

		if (!(file << std::ifstream(filePath.getData())))
			return SceneFileParsingError::COULD_NOT_READ_FILE;

		file.replaceAll('\n', ' ');
		file.replaceAll('\t', ' ');

		size_t versionKeyIndex = file.findFirstOf("version:");
		size_t versionStartIndex = file.findFirstNotOf(' ', versionKeyIndex + strlen("version:") + 1);
		String version = file.subString(versionStartIndex, file.findFirstOf(' ', versionStartIndex + 1));

		String sceneName = filePath.subString(filePath.findLastOf('/') + 1, filePath.findFirstOf('.'));

		if (version == "0.0.1")
			return _parseVersion0_0_1(sceneName, version, file);
		else
			return SceneFileParsingError::INVALID_FILE_VERSION;
	}

	void SceneSerializer::generateCode(const SceneFileData& data)
	{
		if (data.version == "0.0.1")
			_generateCodeVersion0_0_1(data);
	}

} // namespace sonic
