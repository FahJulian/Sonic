#include <fstream>
#include <algorithm>
#include "Sonic/Log/Log.h"
#include "CursorLoader.h"

static const int CUR_HEADER_SIZE = 22;

struct CursorData
{
	uint16_t fileType;
	uint16_t cursorAmount;
	uint8_t imageWidth;
	uint8_t imageHeight;
	uint16_t cursorOffsetX;
	uint16_t cursorOffsetY;
	uint16_t bitsPerPixel;
	uint8_t* imageData;

	~CursorData()
	{
		delete[] imageData;
	}
};

namespace Sonic {
	
	namespace Util {

		static void extractCursorData(std::ifstream& fileStream, CursorData* data)
		{
			uint32_t bmpOffset;
			uint32_t imageSize;

			fileStream.seekg(2, std::ios::cur);
			fileStream.read((char*)&data->fileType, sizeof(data->fileType));
			fileStream.read((char*)&data->cursorAmount, sizeof(data->cursorAmount));
			fileStream.read((char*)&data->imageWidth, sizeof(data->imageWidth));
			fileStream.read((char*)&data->imageHeight, sizeof(data->imageHeight));
			fileStream.seekg(2, std::ios::cur);
			fileStream.read((char*)&data->cursorOffsetX, sizeof(data->cursorOffsetX));
			fileStream.read((char*)&data->cursorOffsetY, sizeof(data->cursorOffsetY));
			fileStream.seekg(4, std::ios::cur);
			fileStream.read((char*)&bmpOffset, sizeof(bmpOffset));

			fileStream.seekg(bmpOffset - CUR_HEADER_SIZE, std::ios::cur);

			fileStream.seekg(14, std::ios::cur);
			fileStream.read((char*)&data->bitsPerPixel, sizeof(data->bitsPerPixel));
			fileStream.seekg(4, std::ios::cur);
			fileStream.read((char*)&imageSize, sizeof(imageSize));
			fileStream.seekg(16, std::ios::cur);

			data->imageData = new uint8_t[imageSize];
			fileStream.read((char*)data->imageData, imageSize);
		}

		static uint8_t* flipDataVertically(const CursorData& data)
		{
			uint8_t* flippedData = new unsigned char[4 * (size_t)data.imageWidth * (size_t)data.imageHeight];

			for (int y = 0; y < data.imageHeight; y++)
			{
				for (int x = 0; x < data.imageWidth; x++)
				{
					uint32_t* pixel = reinterpret_cast<uint32_t*>(data.imageData + 4 * (x + y * data.imageWidth));
					uint32_t* flippedPixel = reinterpret_cast<uint32_t*>(flippedData + 4 * (x + (data.imageHeight - 1 - y) * data.imageWidth));

					*flippedPixel = *pixel;
				}
			}

			return flippedData;
		}

		unsigned char* loadCursor(std::string filePath, int& width, int& height, int& offsetX, int& offsetY)
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
			extractCursorData(file, &data);

			if (data.fileType != 2)
			{
				SONIC_LOG_ERROR("Error loading cursor bitmap: File is not a .cur file");
				return nullptr;
			}

			if (data.cursorAmount != 1)
				SONIC_LOG_WARN("Cursor file contains multiple images. Loading the first one");

			uint8_t* flippedData = flipDataVertically(data);

			width = data.imageWidth;
			height = data.imageHeight;
			offsetX = data.cursorOffsetX;
			offsetY = data.cursorOffsetY;

			return flippedData;
		}

	}

}
