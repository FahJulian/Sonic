#include "FileUtils.h"

#include <Windows.h> 

namespace sonic::platform
{
	bool directoryExists(const String& directoryPath)
	{
		auto fileAttributes = GetFileAttributesA(directoryPath.getData());
		return fileAttributes != INVALID_FILE_ATTRIBUTES && (fileAttributes & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool fileExists(const String& filePath)
	{
		auto fileAttributes = GetFileAttributesA(filePath.getData());
		return fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY);
	}

	bool createDirectory(const String& directory)
	{
		return CreateDirectoryA(directory.getData(), NULL);
	}

	bool moveFile(const String& oldName, const String& newName)
	{
		return MoveFileA(oldName.getData(), newName.getData());
	}

	bool renameFile(const String& oldName, const String& newName)
	{
		return moveFile(oldName, newName);
	}

	bool createDirectoryRecursively(const String& directory)
	{
		for (size_t index = directory.findFirstOf('\\', directory.findFirstOf(":\\") + 1), end = directory.getSize();
			index < end; index = directory.findFirstOf('\\', index + 1))
		{
			String path = directory.subString(0, index + 1);
			if (!directoryExists(path) && !createDirectory(path))
				return false;
		}

		return true;
	}

	bool deleteFile(const String& fileName)
	{
		return DeleteFileA(fileName.getData());
	}

	bool deleteEmptyDirectory(const String& fileName)
	{
		return RemoveDirectoryA(fileName.getData());
	}

	bool deleteEmptyDirectoryRecursively(const String& directory)
	{
		for (size_t index = directory.findLastOf('\\'), end = directory.findFirstOf(":\\");
			index > end + 1; index = directory.findLastOf('\\', index - 1))
		{
			String path = directory.subString(0, index + 1);

			if (!directoryExists(path))
				continue;

			if (!deleteEmptyDirectory(path))
				return false;
		}

		return true;
	}

} // namespace sonic::platform
