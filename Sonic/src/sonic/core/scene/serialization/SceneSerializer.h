#pragma once

#include "sonic/util/Map.h"
#include "sonic/util/String.h"
#include "sonic/util/Result.h"

namespace sonic
{
	using SceneData = Map<String, Map<String, String>>;

	enum class SceneFileParsingError
	{
		COULD_NOT_READ_FILE,
		INVALID_FILE_TYPE,
		INVALID_FILE_VERSION
	};

	class SceneSerializer
	{
	public:
		static Result<SceneData, SceneFileParsingError> parse(const String& sceneFile);
	};

} // namespace sonic
