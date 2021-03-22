#pragma once

#include "sonic/util/Map.h"
#include "sonic/util/String.h"
#include "sonic/util/Result.h"

namespace sonic
{
	struct SceneFileData
	{
		String name;
		String version;
		Map<String, Map<String, String>> data;
	};

	enum class SceneFileParsingError
	{
		COULD_NOT_READ_FILE,
		INVALID_FILE_VERSION,
		SYNTAX_ERROR_REACHED_EOF
	};

	class SceneSerializer
	{
	public:
		static Result<SceneFileData, SceneFileParsingError> parse(const String& sceneFile);

		static void generateCode(const SceneFileData& data);
	};

} // namespace sonic
