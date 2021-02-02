#include <fstream>
#include <algorithm>
#include "Sonic/Log/Log.h"
#include "CursorLoader.h"

static const uint16_t CUR_FILE_TYPE = 2;
static const uint32_t CUR_HEADER_SIZE = 22;

struct CursorData
{
	uint8_t imageWidth;
	uint8_t imageHeight;
	uint16_t cursorOffsetX;
	uint16_t cursorOffsetY;
	uint8_t* imageData;
};

namespace Sonic {
	
	namespace Util {

		static bool extractCursorData(std::ifstream& fileStream, CursorData* data)
		{
			uint16_t fileType;
			uint16_t cursorAmount;
			uint32_t bmpOffset;
			uint16_t bitsPerPixel;
			uint32_t imageSize;

			fileStream.seekg(2, std::ios::cur);
			fileStream.read((char*)&fileType, sizeof(fileType));

			if (fileType != CUR_FILE_TYPE)
			{
				SONIC_LOG_ERROR("Error loading cursor bitmap: File is not a .cur file");
				return false;
			}

			fileStream.read((char*)&cursorAmount, sizeof(cursorAmount));

			if (cursorAmount != 1)
				SONIC_LOG_WARN("Cursor file contains multiple images. Loading the first one");

			fileStream.read((char*)&data->imageWidth, sizeof(data->imageWidth));
			fileStream.read((char*)&data->imageHeight, sizeof(data->imageHeight));
			fileStream.seekg(2, std::ios::cur);
			fileStream.read((char*)&data->cursorOffsetX, sizeof(data->cursorOffsetX));
			fileStream.read((char*)&data->cursorOffsetY, sizeof(data->cursorOffsetY));
			fileStream.seekg(4, std::ios::cur);
			fileStream.read((char*)&bmpOffset, sizeof(bmpOffset));

			fileStream.seekg(bmpOffset - CUR_HEADER_SIZE, std::ios::cur);

			fileStream.seekg(14, std::ios::cur);
			fileStream.read((char*)&bitsPerPixel, sizeof(bitsPerPixel));

			if (bitsPerPixel != 32)
			{
				SONIC_LOG_ERROR("Error loading cursor bitmap: Bitmap does not use 32 bits per pixel");
				return false;
			}

			fileStream.seekg(4, std::ios::cur);
			fileStream.read((char*)&imageSize, sizeof(imageSize));
			fileStream.seekg(16, std::ios::cur);

			data->imageData = new uint8_t[imageSize];
			fileStream.read((char*)data->imageData, imageSize);

			return true;
		}

		static uint8_t* flipDataVertically(const CursorData& data)
		{
			uint8_t* flippedData = new unsigned char[4 * (size_t)data.imageWidth * (size_t)data.imageHeight];

			for (int y = 0; y < data.imageHeight; y++)
			{
				for (int x = 0; x < data.imageWidth; x++)
				{
					uint32_t* pixel = reinterpret_cast<uint32_t*>(data.imageData + 4 * (x + y * (size_t)data.imageWidth));
					uint32_t* flippedPixel = reinterpret_cast<uint32_t*>(flippedData + 4 * (x + ((size_t)data.imageHeight - 1 - y) * (size_t)data.imageWidth));

					*flippedPixel = *pixel;
				}
			}

			return flippedData;
		}

		unsigned char* loadCursor(std::string filePath, int& width, int& height, int& offsetX, int& offsetY, bool flipVertically)
		{
			std::replace(filePath.begin(), filePath.end(), '/', '\\');

			std::ifstream file;
			file.open(filePath, std::ios::in | std::ios::binary);

			if (!file.is_open())
			{
				SONIC_LOG_ERROR("Error loading cursor bitmap: Could not open file");
				return nullptr;
			}

			CursorData data;
			bool error = !extractCursorData(file, &data);

			if (error)
				return nullptr;

			if (flipVertically)
			{
				uint8_t* flippedData = flipDataVertically(data);
				delete[] data.imageData;
				data.imageData = flippedData;
			}

			width = data.imageWidth;
			height = data.imageHeight;
			offsetX = data.cursorOffsetX;
			offsetY = data.cursorOffsetY;

			return data.imageData;
		}

	}

}
