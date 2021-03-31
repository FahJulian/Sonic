#pragma once

#include "sonic/util/String.h"

namespace sonic::platform
{
	bool directoryExists(const String& directoryPath);
	bool fileExists(const String& filePath);

	bool createDirectory(const String& directory);
	bool createDirectoryRecursively(const String& directory);

	bool moveFile(const String& oldName, const String& newName);
	bool renameFile(const String& oldName, const String& newName);

	bool deleteFile(const String& fileName);
	bool deleteEmptyDirectory(const String& directoryPath);
	bool deleteEmptyDirectoryRecursively(const String& directoryPath);

} // namespace sonic::platform
