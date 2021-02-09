#include <cstring>
#include "ComponentType.h"

using namespace Sonic;


static ComponentType s_NextType = 0;

static size_t s_SizesSize;
static size_t* s_Sizes;


ComponentType Sonic::registerComponent(size_t componentSize)
{
	ComponentType type = s_NextType++;
	size_t* newSizes = new size_t[s_SizesSize + 1];
	memcpy(newSizes, s_Sizes, s_SizesSize * sizeof(size_t));
	newSizes[type] = componentSize;
	delete[] s_Sizes;
	s_Sizes = newSizes;
	s_SizesSize++;
	return type;
}

size_t Sonic::getComponentSize(ComponentType type)
{
	return s_Sizes[type];
}
